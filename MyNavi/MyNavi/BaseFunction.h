/**
 * @file BaseFunction.h
 * @brief ��������
 * @details �����ǶȻ�������������ϵ�任��������������
 * @author QYC
 * @version v1.0
 * @date 2024/6/28
 * @copyright QYC
 */
#pragma once
#include"BaseStruct.h"
#include"Eigen/Dense"
#include <math.h>
using namespace Eigen;
#pragma region �ǶȻ�������
//�ǶȻ�������
/**
 * @brief					����radת��Ϊ�Ƕ�deg
 * @param a					����Ļ��Ƚ�
 * @return					�ǶȽ�
 * @note
 */
double rad2deg(double a);
/**
 * @brief					�Ƕ�degת��Ϊ����rad
 * @param a					����ĽǶȽ�
 * @return					���Ƚ�
 * @note
 */
double deg2rad(double a);
/**
 * @brief					ŷ����ת��Ϊ��ת����
 * @param yaw				z�ẽ���
 * @param roll				y������
 * @param pitch				x�ḩ����
 * @return					��ת����
 * @note
 */
Matrix3d Euler2RotationMatrix(const double yaw, const double roll, const double pitch);
/**
 * @brief					ŷ����ת��Ϊ��ת����
 * @param att				��̬��
 *							att[0]-z�ẽ��att[1]-y������att[2]-x�ḩ��
 * @return					��ת����
 * @note
 */
Matrix3d Euler2RotationMatrix(const double* att);
/**
 * @brief					��ת����ת��Ϊŷ����
 * @param R					��ת����
 * @param yaw				z�ẽ���
 * @param roll				y������
 * @param pitch				x�ḩ����
 * @note
 */
void RotationMatrix2Euler(const Matrix3d& R, double& yaw, double& roll, double& pitch);
/**
 * @brief					��ת����תΪŷ����
 * @param R					��ת����
 * @param att				��̬��
 *							att[0]-z�ẽ��att[1]-y������att[2]-x�ḩ��
 * @note
 */
void RotationMatrix2Euler(const Matrix3d& R, double* att);
/**
 * @brief					����eϵ��Lϵ����ת����
 */
Matrix3d GetReL(const double xyz[], EPType ep_type);
#pragma endregion
#pragma region ����ϵ�任
/**
 * @brief					����ֱ������ϵXYZת������γ��BLH
 * @param x					x(m)
 * @param y					y(m)
 * @param z					z(m)
 * @param b					γ��(rad)
 * @param l					����(rad)
 * @param h					�߳�(m)
 * @param ep_type			ʹ�õ��������
 * @note
 */
void XYZ2BLH(double x, double y, double z, double& b, double& l, double& h, EPType ep_type);
/**
 * @brief					����ֱ������ϵXYZת������γ��BLH
 * @param xyz				����ֱ������ϵXYZ
 * @param blh				��γ��BLH
 * @param ep_type			ʹ�õ��������
 * @note
 */
void XYZ2BLH(const double* xyz, double* blh, EPType ep_type);
/**
 * @brief					��γ��BLHת��������ֱ������ϵXYZ
 * @param blh				��γ��BLH
 * @param xyz				����ֱ������ϵXYZ
 * @param ep_type			ʹ�õ��������
 * @note
 */
void BLH2XYZ(const double* blh, double* xyz, EPType ep_type);
#pragma endregion
#pragma region ��������
/**
 * @brief					��ȡ���ص�����ֵ
 * @param b					γ��(rad)
 * @param h					�߳�(m)
 * @return					����ֵm/s/s
 * @note
 */
double GetGravity(const double b, const double h);
/**
 * @brief					��ȡ��ά�����ķ��Գƾ���
 * @param v					������ά����
 * @return					���Գƾ���
 * @note
 */
Matrix3d Skew(double* v);
#pragma endregion