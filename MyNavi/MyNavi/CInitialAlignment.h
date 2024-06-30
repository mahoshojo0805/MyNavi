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
	 * @brief					��ʼ��׼
	 * @param AlignTime			��ʼ��׼������ʱ��
	 * @param att				eϵ��bϵ��̬
	 * @param ImuFilepath		
	 * @param b					γ��
	 * @param h					�߳�
	 * @param StartTime			��ʼ�����ʱ��
	 * @note
	 */
	static void InitalAlign(GPSTime& AlignTime, double att[3], const char* ImuFilepath, const double blh[3], const double StartWeek, const double StartSow);
private:
	/**
	* @brief					�������͵�λ������W�����V����
	* @param g					����g
	* @param w					���ٶ�w
	* @param WV				[out]���
	* @note
	*/
	static Matrix3d GetWV(double* g, double* w);
	/**
	 * @brief					������̬����
	 * @param V
	 * @param W
	 * @param b					γ��
	 * @param l					����
	 * @param att				[out]���
	 * @note
	 */
	static void GetAtt(const Matrix3d* V, const Matrix3d* W, double* att);
};