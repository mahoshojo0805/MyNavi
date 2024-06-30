/**
 * @file BaseStruct.h
 * @brief					时间、角度、位置等结构体的定义
 * @details
 * @author QYC
 * @version v
 * @date 2024/6/2
 * @copyright QYC
 */
#pragma once
#define PI 3.14159265358979323846		///<圆周率

#define OMEGA_EARTH	7.292115E-5			///<地球自转角速度，单位rad/s

#define GYOSCALE 1.0/160849.54386300	///<陀螺仪比例因子
#define ACCSCALE 1.0/655360.00000000	///<加速度计比例因子
#define IMUFREQ 100						///<IMU频率

 /**
  * @brief	椭球参数
  */
enum EPType { WGS84, CGCS2000 };
static struct EP {
	double f, a;//扁率，长半轴
	double e2;
	EP(double F, double A)
	{
		f = F; a = A;
		e2 = 2 * f - f * f;
	}
}tagWGS84(1.0 / 298.257223563, 6378137.0),
tagCGCS2000(1.0 / 298.257222101, 6378137.0);

/**
 * @brief	GPS时
 */
struct GPSTime
{
	int week;
	double sow;
	GPSTime(int w = 0, double s = 0) { week = w; sow = s; }
	//操作符重载
	GPSTime operator = (const GPSTime& t)
	{
		week = t.week;
		sow = t.sow;
		return *this;
	}
	GPSTime operator - (const double dt) const
	{
		GPSTime out;
		out = *this;
		out.sow -= dt;
		while (out.sow >= 604800) { out.week++; out.sow -= 604800.0; }
		while (out.sow < 0) { out.week--; out.sow += 604800.0; }
	}
	double operator - (const GPSTime& t) const
	{
		double out;
		double week = this->week - t.week;
		double sow = this->sow - t.sow;
		out = week * 604800 + sow;
		return out;
	}
	bool operator < (const GPSTime& T)
	{
		if (week < T.week)return true;
		if (week > T.week)return false;
		if (sow < T.sow)return true;
		return false;
	}
	bool operator <= (const GPSTime& T)
	{
		if (week < T.week)return true;
		if (week > T.week)return false;
		if (sow <= T.sow)return true;
		return false;
	}
	bool operator == (const GPSTime& T)
	{
		if (week == T.week && sow == T.sow)return true;
		return false;
	}
};
/**
 * @brief	运动状态
 */
struct NavState
{
	GPSTime time;//时间
	double att[3];//!!e系到b系的旋转!!  姿态	Z(yaw)-Y(roll)-X(pitch) 单位：rad
	double pos[3];//位置XYZ 单位：m
	double vel[3];//速度XYZ 单位：m/s
	//运算符重载
	NavState operator = (const NavState& a)
	{
		time = a.time;
		for (int i = 0; i < 3; i++)
		{
			att[i] = a.att[i];
			pos[i] = a.pos[i];
			vel[i] = a.vel[i];
		}
		return *this;
	}
};
/**
 * @brief	单个历元的IMU数据
 */
struct IMUData
{
	GPSTime time;
	double wx, wy, wz;//<三个方向的角速度
	double fx, fy, fz;//<三个方向的比力
};