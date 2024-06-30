#pragma once
#include"BaseStruct.h"
#include"BaseFunction.h"
#include"Eigen/Dense"
#include<iostream>
using namespace Eigen;
class CInsMech
{
public:
	/**
	 * @brief					姿态更新
	 * @param navstate
	 * @param laststate
	 * @param imu_k
	 * @param dt
	 * @note
	 */
	void static UpdateAtt(NavState* navstate, const NavState* laststate, const IMUData* imu_k, const double dt);
	/**
	 * @brief					速度更新
	 * @param curstate
	 * @param laststate
	 * @param imu_k
	 * @param ge
	 * @param dt
	 * @note
	 */
	void static UpdateVel(NavState* curstate, const NavState* laststate, const IMUData* imu_k, const double* ge, const double dt);
	/**
	 * @brief					位置更新
	 * @param curstate
	 * @param laststate
	 * @param dt
	 * @note
	 */
	void static UpdatePos(NavState* curstate, const NavState* laststate, const double dt);
};
