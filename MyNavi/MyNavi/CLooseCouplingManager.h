#pragma once
#include"BaseIMUReader.h"
#include"BaseGNSSReader.h"
#include "CInsLooseCoupling.h"
#include"CInitialAlignment.h"
#include<string>
using namespace std;
class CLooseCouplingManager
{
private:
	bool m_bImuFile, m_bGnssFile, m_bStdData, m_bLever,
		m_bInitPos, m_bInitVel, m_bInitAtt, m_bL2ERotAngle, m_bRandomWalk,
		m_bStartTime;
	double m_dPosStd[3], m_dVelStd[3], m_dAttStd[3];
	double m_dlever[3];
	double m_dInitPos[3], m_dInitVel[3], m_dInitAtt[3];
	double m_dL2ERotAngle[3];
	double m_dARW, m_dVRW;
	GPSTime m_tagAlignTime, m_tagStartTime;
	string m_strImuPath, m_strGnssPath;
	struct IMUDataNode {
		IMUData ImuData;
		IMUDataNode* Next;
		IMUDataNode() { Next = NULL; }
	}*m_pImuDataHead;
	struct GNSSDataNode{
		NavState GnssState, GnssStd;
		GNSSDataNode* Next;
		GNSSDataNode() { Next = NULL; }
	}*m_pGnssDataHead;
	struct NavStateNode {
		NavState Navstate;
		NavStateNode* Next;
		NavStateNode() { Next = NULL; }
	}*m_pNavStateHead;
public:
	/**
	 * @brief					设置杆臂
	 * @param x
	 * @param y
	 * @param z
	 * @note
	 */
	void SetLever(const double lever[3]); void SetLever(const double x, const double y, const double z);
	/**
	 * @brief					设置初始误差协方差P阵
	 * @param PosStd
	 * @param VelStd
	 * @param AttStd
	 * @note
	 */
	void SetStdData(const double PosStd[3], const double VelStd[3], const double AttStd[3]);
	/**
	 * @brief					设置IMU文件路径
	 * @param filepath
	 * @note
	 */
	void SetImuFilePath(const char* filepath);
	/**
	 * @brief					设置GNSS文件路径
	 * @param filepath
	 * @note
	 */
	void SetGnssFilePath(const char* filepath);
	/**
	 * @brief					设置开始解算的时间
	 * @param Week
	 * @param Sow
	 * @note
	 */
	void SetStartTime(const double Week, const double Sow);
	/**
	 * @brief					设置初始位置
	 * @param x
	 * @param y
	 * @param z
	 * @note
	 */
	void SetInitPos(const double InitPos[3]); void SetInitPos(const double x, const double y, const double z);
	/**
	 * @brief					设置初始速度
	 * @param x
	 * @param y
	 * @param z
	 * @note
	 */
	void SetInitVel(const double InitVel[3]); void SetInitVel(const double x, const double y, const double z);
	/**
	 * @brief					设置初始姿态
	 * @param x
	 * @param y
	 * @param z
	 * @note
	 */
	void SetInitAtt(const double InitAtt[3]); void SetInitAtt(const double x, const double y, const double z);
	/**
	 * @brief					设置载体坐标系到IMU坐标系的旋转角
	 * @param x
	 * @param y
	 * @param z
	 * @note
	 */
	void SetBody2IMUAngle(const double Angle[3]); void SetBody2IMUAngle(const double x, const double y, const double z);
	/**
	 * @brief					设置随机过程噪声
	 * @param ARW
	 * @param VRW
	 * @note
	 */
	void SetRandomWalk(const double ARW, const double VRW);
	/**
	 * @brief					运行纯惯导
	 * @note
	 */
	void RunPureIns();
	/**
	 * @brief					运行松组合
	 * @note
	 */
	void RunLC();
	/**
	 * @brief					将解算结果写入文件
	 * @param filepath
	 * @note
	 */
	void WriterResultInFile(const char* filepath);
	/**
	 * @brief					清除IMU/GNSS/解算结果链表中所有元素
	 * @note
	 */
	void Clear();
};

