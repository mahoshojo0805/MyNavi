#pragma once
#include"BaseStruct.h"
#include"BaseFunction.h"
#include"Eigen/Dense"
#include"CInsMech.h"
#define RANK 15//���״̬ά��
using namespace Eigen;
class CInsLooseCoupling
{
public:
	//NavState m_tagCurState, m_LastState;
	NavState m_tagGnssState, m_tagGnssCov;
	double m_ddt;//����IMU֮��ļ��
	//bool m_bIsImuEnough;//IMU�����Ƿ��㹻
	IMUData m_tagImuData;
	double m_dlever[3];//bϵ�ĸ˱�
	VectorXd m_vdx;//���״̬��
	double m_dAccBias[3], m_dGyrBias[3];//���ٶȼƺ�������ƫ
	Matrix3d m_mIta_a, m_mIta_v;
	MatrixXd m_mP;
public:
	/**
	 * @brief					����GNSS����
	 * @param input_gnss		�����GNSS�۲�
	 * @param input_cov			�����GNSSЭ����
	 * @note
	 */
	void AddGnssData(const NavState& input_gnss, const NavState& input_cov);
	/**
	 * @brief					����IMU����
	 * @param input				�����IMU����
	 * @note
	 */
	void AddImuData(const IMUData& input);
	/**
	 * @brief					�˲���ʼ��
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
	 * @brief					�ߵ���е����
	 * @param Curstate
	 * @param Laststate
	 * @note
	 */
	void INSMech(NavState& Curstate, const NavState& Laststate);
	/**
	 * @brief					INS���״̬Ԥ��
	 * @param CurState
	 * @note
	 */
	void INSPredict(const NavState& CurState);
	/**
	 * @brief					GNSS״̬Ԥ��
	 * @param CurState
	 * @note
	 */
	void GNSSUpdate(const NavState& CurState);
	/**
	 * @brief					����IMU���
	 * @note
	 */
	void CompensateIMU();
	/**
	 * @brief					����
	 * @param CurState
	 * @note
	 */
	void FeedBack(NavState& CurState);
	/**
	 * @brief					�ߵ��ڲ�
	 * @param ImuData			��ǰ��Imu����
	 * @return					��ֵ�õ���imu���
	 * @note
	 */
	static IMUData Interpolate(IMUData& ImuData, const GPSTime Time);
};

