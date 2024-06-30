#pragma once
#include"BaseGNSSReader.h"
#include"BaseIMUReader.h"
#include"BaseStruct.h"
#define THRESHOLD_ACC 0.2
#define THRESHOLD_GYR 0.02

class CInitialAlignment
{
public:
	/**
	 * @brief					初始对准
	 * @param AlignTime			初始对准结束的时间
	 * @param att				e系到b系姿态
	 * @param ImuFilepath		
	 * @param b					纬度
	 * @param h					高程
	 * @param StartTime			开始计算的时间
	 * @note
	 */
	static void InitalAlign(GPSTime& AlignTime, double att[3], const char* ImuFilepath, const double blh[3], const double StartWeek, const double StartSow);
private:
	/**
	* @brief					正交化和单位化计算W矩阵或V矩阵
	* @param g					重力g
	* @param w					角速度w
	* @param WV				[out]结果
	* @note
	*/
	static Matrix3d GetWV(double* g, double* w);
	/**
	 * @brief					计算姿态矩阵
	 * @param V
	 * @param W
	 * @param b					纬度
	 * @param l					经度
	 * @param att				[out]输出
	 * @note
	 */
	static void GetAtt(const Matrix3d* V, const Matrix3d* W, double* att);
};