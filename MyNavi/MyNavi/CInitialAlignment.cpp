#include "pch.h"
#include "CInitialAlignment.h"
void CInitialAlignment::InitalAlign(GPSTime& AlignTime, double att[3], const char* ImuFilepath, const double blh[3], const double StartWeek, const double StartSow)
{
	IMUData ImuData;
	BaseIMUReader ImuReader;
	GPSTime StartTime;
	StartTime.week = StartWeek; StartTime.sow = StartSow;
	ImuReader.Openfile(ImuFilepath);
	//计算平均值用于初始对准
	double mean_fx = 0, mean_fy = 0, mean_fz = 0;
	double mean_wx = 0, mean_wy = 0, mean_wz = 0;
	int count = 0;
	double g = GetGravity(blh[0], blh[2]);
	while (ImuReader.Next(ImuData))
	{
		if (ImuData.time < StartTime)continue;
		//判断是否在静止时间段内
		if (abs(sqrt(pow(ImuData.fx, 2) + pow(ImuData.fy, 2) + pow(ImuData.fz, 2)) - g) > THRESHOLD_ACC
			|| abs(sqrt(pow(ImuData.wx, 2) + pow(ImuData.wy, 2) + pow(ImuData.wz, 2))) > THRESHOLD_GYR)
			break;
		//求均值
		mean_fx = ((double)count) / (count + 1) * mean_fx + ImuData.fx / (count + 1);
		mean_fy = ((double)count) / (count + 1) * mean_fy + ImuData.fy / (count + 1);
		mean_fz = ((double)count) / (count + 1) * mean_fz + ImuData.fz / (count + 1);
		mean_wx = ((double)count) / (count + 1) * mean_wx + ImuData.wx / (count + 1);
		mean_wy = ((double)count) / (count + 1) * mean_wy + ImuData.wy / (count + 1);
		mean_wz = ((double)count) / (count + 1) * mean_wz + ImuData.wz / (count + 1);
		count++;
		AlignTime = ImuData.time;
		if (count >= 30000)
			break;
	}
	double gL[3] = { 0,0,-g };
	double wL[3] = { 0,OMEGA_EARTH * cos(blh[0]),OMEGA_EARTH * sin(blh[0]) };
	double gb[3] = { -mean_fx,-mean_fy,-mean_fz };
	double wb[3] = { mean_wx,mean_wy,mean_wz };

	Matrix3d W, V;
	V = GetWV(gL, wL);
	W = GetWV(gb, wb);
	GetAtt(&V, &W, att);
	att[0] = rad2deg(att[0]);
	att[1] = rad2deg(att[1]);
	att[2] = rad2deg(att[2]);

	ImuReader.Closefile();
}
Matrix3d CInitialAlignment::GetWV(double* g, double* w)
{
	Matrix3d WV;
	Vector3d vecg(g[0], g[1], g[2]);
	Vector3d vecw(w[0], w[1], w[2]);
	Vector3d vg = vecg;
	Vector3d vw = vecg.cross(vecw);
	Vector3d vgw = vw.cross(vecg);
	vg.normalize();
	vw.normalize();
	vgw.normalize();
	WV.block<1, 3>(0, 0) = vg;
	WV.block<1, 3>(1, 0) = vw;
	WV.block<1, 3>(2, 0) = vgw;
	return WV;
}
void CInitialAlignment::GetAtt(const Matrix3d* V, const Matrix3d* W, double* att)
{
	Matrix3d Ceb = ((V->inverse()) * (*W)).transpose();
	RotationMatrix2Euler(Ceb, att[0], att[1], att[2]);
}