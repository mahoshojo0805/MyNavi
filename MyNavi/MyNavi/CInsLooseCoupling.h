#pragma once
#include"BaseStruct.h"
#include"BaseFunction.h"
#include"Eigen/Dense"
#include"CInsMech.h"
#define RANK 15//误差状态维度
using namespace Eigen;
class CInsLooseCoupling
{
public:
	//NavState m_tagCurState, m_LastState;
	NavState m_tagGnssState, m_tagGnssCov;
	double m_ddt;//两次IMU之间的间隔
	//bool m_bIsImuEnough;//IMU数据是否足够
	IMUData m_tagImuData;
	double m_dlever[3];//b系的杆臂
	VectorXd m_vdx;//误差状态量
	double m_dAccBias[3], m_dGyrBias[3];//加速度计和陀螺零偏
	Matrix3d m_mIta_a, m_mIta_v;
	MatrixXd m_mP;
public:
	/**
	 * @brief					输入GNSS数据
	 * @param input_gnss		输入的GNSS观测
	 * @param input_cov			输入的GNSS协方差
	 * @note
	 */
	void AddGnssData(const NavState& input_gnss, const NavState& input_cov);
	/**
	 * @brief					输入IMU数据
	 * @param input				输入的IMU数据
	 * @note
	 */
	void AddImuData(const IMUData& input);
	/**
	 * @brief					滤波初始化
	 * @param ARW
	 * @param VRW
	 * @param lever
	 * @param pos_std
	 * @param vel_std
	 * @param att_std
	 * @note
	 */
	void Initialize(const double ARW, const double VRW, const double lever[],
		const double pos_std[], const double vel_std[], const double att_std[]);
	/**
	 * @brief					惯导机械编排
	 * @param Curstate
	 * @param Laststate
	 * @note
	 */
	void INSMech(NavState& Curstate, const NavState& Laststate);
	/**
	 * @brief					INS误差状态预测
	 * @param CurState
	 * @note
	 */
	void INSPredict(const NavState& CurState);
	/**
	 * @brief					GNSS状态预测
	 * @param CurState
	 * @note
	 */
	void GNSSUpdate(const NavState& CurState);
	/**
	 * @brief					补偿IMU误差
	 * @note
	 */
	void CompensateIMU();
	/**
	 * @brief					误差反馈
	 * @param CurState
	 * @note
	 */
	void FeedBack(NavState& CurState);
	/**
	 * @brief					惯导内插
	 * @param ImuData			当前的Imu数据
	 * @return					插值得到的imu输出
	 * @note
	 */
	static IMUData Interpolate(IMUData& ImuData, const GPSTime Time);
};

