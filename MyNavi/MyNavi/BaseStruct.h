/**
 * @file BaseStruct.h
 * @brief					ʱ�䡢�Ƕȡ�λ�õȽṹ��Ķ���
 * @details
 * @author QYC
 * @version v
 * @date 2024/6/2
 * @copyright QYC
 */
#pragma once
#define PI 3.14159265358979323846		///<Բ����

#define OMEGA_EARTH	7.292115E-5			///<������ת���ٶȣ���λrad/s

#define GYOSCALE 1.0/160849.54386300	///<�����Ǳ�������
#define ACCSCALE 1.0/655360.00000000	///<���ٶȼƱ�������
#define IMUFREQ 100						///<IMUƵ��

 /**
  * @brief	�������
  */
enum EPType { WGS84, CGCS2000 };
static struct EP {
	double f, a;//���ʣ�������
	double e2;
	EP(double F, double A)
	{
		f = F; a = A;
		e2 = 2 * f - f * f;
	}
}tagWGS84(1.0 / 298.257223563, 6378137.0),
tagCGCS2000(1.0 / 298.257222101, 6378137.0);

/**
 * @brief	GPSʱ
 */
struct GPSTime
{
	int week;
	double sow;
	GPSTime(int w = 0, double s = 0) { week = w; sow = s; }
	//����������
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
 * @brief	�˶�״̬
 */
struct NavState
{
	GPSTime time;//ʱ��
	double att[3];//!!eϵ��bϵ����ת!!  ��̬	Z(yaw)-Y(roll)-X(pitch) ��λ��rad
	double pos[3];//λ��XYZ ��λ��m
	double vel[3];//�ٶ�XYZ ��λ��m/s
	//���������
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
 * @brief	������Ԫ��IMU����
 */
struct IMUData
{
	GPSTime time;
	double wx, wy, wz;//<��������Ľ��ٶ�
	double fx, fy, fz;//<��������ı���
};