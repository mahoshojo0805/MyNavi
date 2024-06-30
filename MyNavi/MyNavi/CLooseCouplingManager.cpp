#include "pch.h"
#include "CLooseCouplingManager.h"

void CLooseCouplingManager::SetLever(const double lever[3])
{
	memcpy(m_dlever, lever, sizeof(double) * 3);
	m_bLever = true;
}
void CLooseCouplingManager::SetLever(const double x, const double y, const double z)
{
	double lever[3] = { x,y,z };
	SetLever(lever);
}
void CLooseCouplingManager::SetStdData(const double PosStd[3], const double VelStd[3], const double AttStd[3])
{
	memcpy(m_dPosStd, PosStd, sizeof(double) * 3);
	memcpy(m_dVelStd, VelStd, sizeof(double) * 3);
	memcpy(m_dAttStd, AttStd, sizeof(double) * 3);
	m_bStdData = true;
}
void CLooseCouplingManager::SetImuFilePath(const char* filepath)
{
	m_strImuPath = filepath;
	m_bImuFile = true;
}
void CLooseCouplingManager::SetGnssFilePath(const char* filepath)
{
	m_strGnssPath = filepath;
	m_bGnssFile = true;
}
void CLooseCouplingManager::SetStartTime(const double Week, const double Sow)
{
	m_tagAlignTime.week = m_tagStartTime.week = Week;
	m_tagAlignTime.sow = m_tagStartTime.sow = Sow;
	m_bStartTime = true;
}
void CLooseCouplingManager::SetInitPos(const double InitPos[3])
{
	memcpy(m_dInitPos, InitPos, sizeof(double) * 3);
	m_bInitPos = true;
}
void CLooseCouplingManager::SetInitPos(const double x, const double y, const double z)
{
	double pos[3] = { x,y,z };
	SetInitPos(pos);
}
void CLooseCouplingManager::SetInitVel(const double x, const double y, const double z)
{
	double vel[3] = { x,y,z };
	SetInitVel(vel);
}
void CLooseCouplingManager::SetInitVel(const double InitVel[3])
{
	memcpy(m_dInitVel, InitVel, sizeof(double) * 3);
	m_bInitVel = true;
}
void CLooseCouplingManager::SetInitAtt(const double InitAtt[3])
{
	memcpy(m_dInitAtt, InitAtt, sizeof(double) * 3);
	m_bInitAtt = true;
}
void CLooseCouplingManager::SetInitAtt(const double x, const double y, const double z)
{
	double att[3] = { x,y,z };
	SetInitAtt(att);
}
void CLooseCouplingManager::SetBody2IMUAngle(const double Angle[3])
{
	memcpy(m_dL2ERotAngle, Angle, sizeof(double) * 3);
	m_bL2ERotAngle = true;
}
void CLooseCouplingManager::SetBody2IMUAngle(const double x, const double y, const double z)
{
	double angle[3] = { x,y,z };
	SetBody2IMUAngle(angle);
}
void CLooseCouplingManager::SetRandomWalk(const double ARW, const double VRW)
{
	m_dARW = ARW;
	m_dVRW = VRW;
	m_bRandomWalk = true;
}
void CLooseCouplingManager::RunPureIns()
{
	if (!m_bInitPos || !m_bImuFile)return;

	NavStateNode* pNavStateNode;
	IMUDataNode* pImuDataNode;
	BaseIMUReader ImuReader;
	BaseGNSSReader GnssReader;
	CInsLooseCoupling LCCaculator;

	double blh[3];
	XYZ2BLH(m_dInitPos, blh, WGS84);

	if (!m_bStartTime)
	{
		m_tagAlignTime.sow = m_tagStartTime.sow = 0;
		m_tagAlignTime.week = m_tagStartTime.week = 0;
	}
	if (!m_bInitAtt)
		CInitialAlignment::InitalAlign(m_tagAlignTime, m_dInitAtt, m_strImuPath.c_str(), blh, m_tagStartTime.week, m_tagStartTime.sow);
	if (!m_bInitVel)
		memset(m_dInitVel, 0, sizeof(double) * 3);
	if (!m_bL2ERotAngle)
		memset(m_dL2ERotAngle, 0, sizeof(double) * 3);

	/*根据初始配置进行相关计算*/
	/*TO DO:载体到IMU的旋转*/

	m_pImuDataHead = new IMUDataNode();
	m_pGnssDataHead = new GNSSDataNode();
	/*打开文件*/
	if (!ImuReader.Openfile(m_strImuPath.c_str()))
		return;
	while (true) {
		ImuReader.Next(m_pImuDataHead->ImuData);
		if (m_tagAlignTime <= m_pImuDataHead->ImuData.time)
			break;
	}
	LCCaculator.AddImuData(m_pImuDataHead->ImuData);

	/*导航状态初值*/
	m_pNavStateHead = new NavStateNode();
	m_pNavStateHead->Navstate.time = m_pImuDataHead->ImuData.time;
	memcpy(m_pNavStateHead->Navstate.pos, m_dInitPos, sizeof(double) * 3);
	memcpy(m_pNavStateHead->Navstate.vel, m_dInitVel, sizeof(double) * 3);
	Matrix3d RLb = Euler2RotationMatrix(deg2rad(m_dInitAtt[2]), deg2rad(m_dInitAtt[1]), deg2rad(m_dInitAtt[0]));
	Matrix3d ReL = GetReL(m_dInitPos, WGS84);
	RotationMatrix2Euler(RLb * ReL, m_pNavStateHead->Navstate.att);
	pNavStateNode = m_pNavStateHead;

	pImuDataNode = m_pImuDataHead;

	while (true)
	{
		IMUDataNode* NewImuNode = new IMUDataNode();
		if (!ImuReader.Next(NewImuNode->ImuData))break;
		pImuDataNode->Next = NewImuNode;
		pImuDataNode = NewImuNode;

		NavStateNode* NewStateNode = new NavStateNode();
		LCCaculator.AddImuData(NewImuNode->ImuData);
		LCCaculator.INSMech(NewStateNode->Navstate, pNavStateNode->Navstate);
		/*保存结果到链表*/
		pNavStateNode->Next = NewStateNode;
		pNavStateNode = NewStateNode;
	}
}
void CLooseCouplingManager::RunLC()
{
	if (!m_bInitPos || !m_bStdData || !m_bImuFile || !m_bGnssFile || !m_bRandomWalk)
		return;

	NavStateNode* pNavStateNode;
	IMUDataNode* pImuDataNode;
	GNSSDataNode* pGnssDataNode;
	BaseIMUReader ImuReader;
	BaseGNSSReader GnssReader;
	CInsLooseCoupling LCCaculator;

	double blh[3];
	XYZ2BLH(m_dInitPos, blh, WGS84);

	/*初始配置*/
	if(!m_bStartTime)
	{
		m_tagStartTime.sow = 0;
		m_tagStartTime.week = 0;
	}
	if (!m_bLever)
		memset(m_dlever, 0, sizeof(double) * 3);
	if (!m_bInitAtt)
		CInitialAlignment::InitalAlign(m_tagAlignTime, m_dInitAtt, m_strImuPath.c_str(), blh, m_tagStartTime.week, m_tagStartTime.sow);
	if (!m_bInitVel)
			memset(m_dInitVel, 0, sizeof(double) * 3);
	if (!m_bL2ERotAngle)
		memset(m_dL2ERotAngle, 0, sizeof(double) * 3);

	/*根据初始配置进行相关计算*/
	/*TO DO:载体到IMU的旋转*/

	m_pImuDataHead = new IMUDataNode();
	m_pGnssDataHead = new GNSSDataNode();
	/*打开文件*/
	if (!ImuReader.Openfile(m_strImuPath.c_str()))
		return;
	if (!GnssReader.Openfile(m_strGnssPath.c_str()))
		return;

	/*添加数据*/
	while (true) {
		ImuReader.Next(m_pImuDataHead->ImuData);
		if (m_tagAlignTime <= m_pImuDataHead->ImuData.time)
			break;
	}
	while (true) {
		GnssReader.Next(m_pGnssDataHead->GnssState, m_pGnssDataHead->GnssStd);
		if (m_tagAlignTime <= m_pGnssDataHead->GnssState.time)
			break;
	}
	LCCaculator.AddGnssData(m_pGnssDataHead->GnssState, m_pGnssDataHead->GnssStd);
	LCCaculator.AddImuData(m_pImuDataHead->ImuData);

	/*噪声参数初值*/
	LCCaculator.Initialize(m_dARW * PI / 180.0 / 60.0, m_dVRW / 60.0, m_dlever, m_dPosStd, m_dVelStd, m_dAttStd);
	/*导航状态初值*/
	m_pNavStateHead = new NavStateNode();
	m_pNavStateHead->Navstate.time = m_pImuDataHead->ImuData.time;
	memcpy(m_pNavStateHead->Navstate.pos, m_dInitPos, sizeof(double) * 3);
	memcpy(m_pNavStateHead->Navstate.vel, m_dInitVel, sizeof(double) * 3);
	Matrix3d RLb = Euler2RotationMatrix(deg2rad(m_dInitAtt[2]), deg2rad(m_dInitAtt[1]), deg2rad(m_dInitAtt[0]));
	Matrix3d ReL = GetReL(m_dInitPos, WGS84);
	RotationMatrix2Euler(RLb * ReL, m_pNavStateHead->Navstate.att);
	pNavStateNode = m_pNavStateHead;

	pImuDataNode = m_pImuDataHead;
	pGnssDataNode = m_pGnssDataHead;


	/*根据不同情况进行松组合解算*/
	while (true)
	{
		IMUDataNode* NewImuNode = new IMUDataNode();
		if (!ImuReader.Next(NewImuNode->ImuData))break;
		pImuDataNode->Next = NewImuNode;
		pImuDataNode = NewImuNode;

		//double dt = pImuDataNode->ImuData.time - LCCaculator.m_tagImuData.time;
		if (pImuDataNode->ImuData.time < pGnssDataNode->GnssState.time)//只做惯导预测
		{
			LCCaculator.AddImuData(pImuDataNode->ImuData);
			NavStateNode* NewStateNode = new NavStateNode();
			LCCaculator.CompensateIMU();
			LCCaculator.INSMech(NewStateNode->Navstate, pNavStateNode->Navstate);
			LCCaculator.INSPredict(NewStateNode->Navstate);
			/*保存结果到链表*/
			pNavStateNode->Next = NewStateNode;
			pNavStateNode = NewStateNode;
		}
		else if (LCCaculator.m_tagImuData.time < pGnssDataNode->GnssState.time
			&& pGnssDataNode->GnssState.time < pImuDataNode->ImuData.time)//插值+惯导预测+Gnss更新
		{
			/*1.惯导状态预测+GNSS更新*/
			IMUData MidImuData = LCCaculator.Interpolate(pImuDataNode->ImuData, pGnssDataNode->GnssState.time);
			LCCaculator.AddImuData(MidImuData);
			NavStateNode* NewStateNode = new NavStateNode();
			LCCaculator.CompensateIMU();
			LCCaculator.INSMech(NewStateNode->Navstate, pNavStateNode->Navstate);
			LCCaculator.INSPredict(NewStateNode->Navstate);
			LCCaculator.GNSSUpdate(NewStateNode->Navstate);
			LCCaculator.FeedBack(NewStateNode->Navstate);
			NavState TempResult;
			memcpy(&TempResult, &NewStateNode->Navstate, sizeof(NewStateNode->Navstate));

			NewStateNode = new NavStateNode();
			/*2.惯导状态预测*/
			LCCaculator.AddImuData(pImuDataNode->ImuData);
			LCCaculator.CompensateIMU();
			LCCaculator.INSMech(NewStateNode->Navstate, TempResult);
			LCCaculator.INSPredict(NewStateNode->Navstate);
			/*保存结果到链表*/
			pNavStateNode->Next = NewStateNode;
			pNavStateNode = NewStateNode;
		}
		else if (pImuDataNode->ImuData.time == pGnssDataNode->GnssState.time)//惯导预测 + Gnss更新
		{
			if (abs(pImuDataNode->ImuData.time.sow - 266072.00)<=0.015) {
				int test = 0;
				test = 0;
			}
			LCCaculator.AddImuData(pImuDataNode->ImuData);
			NavStateNode* NewStateNode = new NavStateNode();
			LCCaculator.CompensateIMU();
			LCCaculator.INSMech(NewStateNode->Navstate, pNavStateNode->Navstate);
			LCCaculator.INSPredict(NewStateNode->Navstate);
			LCCaculator.GNSSUpdate(NewStateNode->Navstate);
			LCCaculator.FeedBack(NewStateNode->Navstate);
			/*保存结果到链表*/
			pNavStateNode->Next = NewStateNode;
			pNavStateNode = NewStateNode;
		}
		bool bIfGnssEnd = false;

		while (pGnssDataNode->GnssState.time <= pImuDataNode->ImuData.time)//是否读取Gnss数据
		{
			GNSSDataNode* NewGnssNode = new GNSSDataNode();
			if (bIfGnssEnd = !GnssReader.Next(NewGnssNode->GnssState, NewGnssNode->GnssStd)) { break; };
			LCCaculator.AddGnssData(NewGnssNode->GnssState, NewGnssNode->GnssStd);
			pGnssDataNode->Next = NewGnssNode;
			pGnssDataNode = NewGnssNode;
		}
		if (bIfGnssEnd)break;
	}
}
void CLooseCouplingManager::WriterResultInFile(const char* filepath)
{
	FILE* fout;
	fopen_s(&fout, filepath, "w");
	NavStateNode* Curstate = m_pNavStateHead;
	while (Curstate->Next != NULL)
	{
		double blh[3], att[3];
		Matrix3d ReL = GetReL(Curstate->Navstate.pos, WGS84);
		Matrix3d Reb = Euler2RotationMatrix(Curstate->Navstate.att);
		RotationMatrix2Euler(Reb * ReL.transpose(), att);
		if (att[0] >= 0)att[0] = 2 * PI - att[0];
		else att[0] = -att[0];
		Vector3d vel = ReL * Vector3d(Curstate->Navstate.vel);
		XYZ2BLH(Curstate->Navstate.pos, blh, WGS84);
		fprintf(fout, "%6d%12.3lf%16.3lf%16.3lf%16.3lf%16.8lf%16.8lf%16.8lf%12.3lf%12.3lf%12.3lf%12.3lf%12.3lf%12.3lf\n",
			Curstate->Navstate.time.week, Curstate->Navstate.time.sow,
			Curstate->Navstate.pos[0], Curstate->Navstate.pos[1], Curstate->Navstate.pos[2],
			rad2deg(blh[0]), rad2deg(blh[1]), blh[2],
			vel[0], vel[1], vel[2],
			rad2deg(att[0]), rad2deg(att[1]), rad2deg(att[2]));
		Curstate = Curstate->Next;
	}
	fclose(fout);
}
void CLooseCouplingManager::Clear()
{
	NavStateNode* Curstate = m_pNavStateHead;
	IMUDataNode* ImuData = m_pImuDataHead;
	GNSSDataNode* GnssData = m_pGnssDataHead;
	while (Curstate->Next != NULL)
	{
		NavStateNode* TempNode = Curstate->Next;
		free(Curstate);
		Curstate = TempNode;
	}
	while (ImuData->Next != NULL)
	{
		IMUDataNode* TempNode = ImuData->Next;
		free(ImuData);
		ImuData = TempNode;
	}
	while (Curstate->Next != NULL)
	{
		GNSSDataNode* TempNode = GnssData->Next;
		free(GnssData);
		GnssData = TempNode;
	}
}
