/**
 * @file BaseFunction.h
 * @brief 基础函数
 * @details 包括角度基本函数、坐标系变换函数和其他函数
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
#pragma region 角度基本函数
//角度基本函数
/**
 * @brief					弧度rad转化为角度deg
 * @param a					输入的弧度角
 * @return					角度角
 * @note
 */
double rad2deg(double a);
/**
 * @brief					角度deg转化为弧度rad
 * @param a					输入的角度角
 * @return					弧度角
 * @note
 */
double deg2rad(double a);
/**
 * @brief					欧拉角转换为旋转矩阵
 * @param yaw				z轴航向角
 * @param roll				y轴横滚角
 * @param pitch				x轴俯仰角
 * @return					旋转矩阵
 * @note
 */
Matrix3d Euler2RotationMatrix(const double yaw, const double roll, const double pitch);
/**
 * @brief					欧拉角转换为旋转矩阵
 * @param att				姿态角
 *							att[0]-z轴航向；att[1]-y轴横滚；att[2]-x轴俯仰
 * @return					旋转矩阵
 * @note
 */
Matrix3d Euler2RotationMatrix(const double* att);
/**
 * @brief					旋转矩阵转换为欧拉角
 * @param R					旋转矩阵
 * @param yaw				z轴航向角
 * @param roll				y轴横滚角
 * @param pitch				x轴俯仰角
 * @note
 */
void RotationMatrix2Euler(const Matrix3d& R, double& yaw, double& roll, double& pitch);
/**
 * @brief					旋转矩阵转为欧拉角
 * @param R					旋转矩阵
 * @param att				姿态角
 *							att[0]-z轴航向；att[1]-y轴横滚；att[2]-x轴俯仰
 * @note
 */
void RotationMatrix2Euler(const Matrix3d& R, double* att);
/**
 * @brief					计算e系到L系的旋转矩阵
 */
Matrix3d GetReL(const double xyz[], EPType ep_type);
#pragma endregion
#pragma region 坐标系变换
/**
 * @brief					地球直角坐标系XYZ转换到经纬高BLH
 * @param x					x(m)
 * @param y					y(m)
 * @param z					z(m)
 * @param b					纬度(rad)
 * @param l					经度(rad)
 * @param h					高程(m)
 * @param ep_type			使用的椭球参数
 * @note
 */
void XYZ2BLH(double x, double y, double z, double& b, double& l, double& h, EPType ep_type);
/**
 * @brief					地球直角坐标系XYZ转换到经纬高BLH
 * @param xyz				地球直角坐标系XYZ
 * @param blh				经纬高BLH
 * @param ep_type			使用的椭球参数
 * @note
 */
void XYZ2BLH(const double* xyz, double* blh, EPType ep_type);
/**
 * @brief					经纬高BLH转换到地球直角坐标系XYZ
 * @param blh				经纬高BLH
 * @param xyz				地球直角坐标系XYZ
 * @param ep_type			使用的椭球参数
 * @note
 */
void BLH2XYZ(const double* blh, double* xyz, EPType ep_type);
#pragma endregion
#pragma region 其他函数
/**
 * @brief					获取当地的重力值
 * @param b					纬度(rad)
 * @param h					高程(m)
 * @return					重力值m/s/s
 * @note
 */
double GetGravity(const double b, const double h);
/**
 * @brief					获取三维向量的反对称矩阵
 * @param v					输入三维向量
 * @return					反对称矩阵
 * @note
 */
Matrix3d Skew(double* v);
#pragma endregion