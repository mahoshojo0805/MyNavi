#include "pch.h"
#include "CInsLooseCoupling.h"
void CInsLooseCoupling::AddGnssData(const NavState& input_gnss, const NavState& input_cov)
{
	memcpy(&m_tagGnssState, &input_gnss, sizeof(input_gnss));
	memcpy(&m_tagGnssCov, &input_cov, sizeof(input_cov));
}
void CInsLooseCoupling::AddImuData(const IMUData& input)
{
	m_ddt = input.time - m_tagImuData.time;
	memcpy(&m_tagImuData, &input, sizeof(input));
}
void CInsLooseCoupling::Initialize(const double ARW, const double VRW, const double lever[],
	const double pos_std[], const double vel_std[], const double att_std[])
{
	m_vdx.resize(RANK);
	m_mP.resize(RANK, RANK);
	m_vdx.setZero();
	m_mP.setZero();
	memset(m_dAccBias, 0, sizeof(double) * 3);
	memset(m_dGyrBias, 0, sizeof(double) * 3);
	/*testÕ”¬›¡„∆´*/
	//m_dGyrBias[0] = -0.00988110874 - 5.7832e-05;
	//m_dGyrBias[1] = -1.773437255998923e-4;// +3.5729e-07;
	//m_dGyrBias[2] = 0.0026016059868 + 3.9816e-05 + 0.00053;
	/*test*/
	memcpy(m_dlever, lever, sizeof(double) * 3);
	double ARW2 = ARW * ARW;
	double VRW2 = VRW * VRW;
	m_mIta_a = Vector3d(ARW2, ARW2, ARW2).asDiagonal();
	m_mIta_v = Vector3d(VRW2, VRW2, VRW2).asDiagonal();
	//≥ı º–≠∑Ω≤Óæÿ’Û‘ı√¥…Ë÷√£ø
	m_mP.block<3, 3>(0, 0) = Vector3d(pos_std[0] * pos_std[0], pos_std[1] * pos_std[1], pos_std[2] * pos_std[2]).asDiagonal();
	m_mP.block<3, 3>(3, 3) = Vector3d(vel_std[0] * vel_std[0], vel_std[1] * vel_std[1], vel_std[2] * vel_std[2]).asDiagonal();
	m_mP.block<3, 3>(6, 6) = Vector3d(att_std[0] * att_std[0], att_std[1] * att_std[1], att_std[2] * att_std[2]).asDiagonal();
}
void CInsLooseCoupling::INSMech(NavState& Curstate, const NavState& Laststate)
{
	double blh[3];
	double g;
	double ge[3];
	XYZ2BLH(Laststate.pos, blh, WGS84);
	g = GetGravity(blh[0], blh[2]);
	ge[0] = -g * cos(blh[0]) * cos(blh[1]);
	ge[1] = -g * cos(blh[0]) * sin(blh[1]);
	ge[2] = -g * sin(blh[0]);
	CInsMech::UpdateAtt(&Curstate, &Laststate, &m_tagImuData, m_ddt);
	/*¡„ÀŸ–ﬁ’˝*/
	double thres_a, thres_w;
	thres_a = 0.01; thres_w = 0.015;
	if (abs(sqrt(pow(m_tagImuData.fx, 2) + pow(m_tagImuData.fy, 2) + pow(m_tagImuData.fz, 2)) - g) > thres_a
		|| sqrt(pow(m_tagImuData.wx, 2) + pow(m_tagImuData.wy, 2) + pow(m_tagImuData.wz, 2)) > thres_w)
		CInsMech::UpdateVel(&Curstate, &Laststate, &m_tagImuData, ge, m_ddt);
	else
		memset(Curstate.vel, 0, sizeof(double) * 3);
	CInsMech::UpdatePos(&Curstate, &Laststate, m_ddt);
	Curstate.time = m_tagImuData.time;
}
void CInsLooseCoupling::INSPredict(const NavState& CurState)
{
	Matrix3d Reb, Rbe;
	double we_ie[3] = { 0,0,OMEGA_EARTH };
	MatrixXd F, PHI, Q, qc;
	Matrix3d;
	Vector3d fb, fe;
	F.resize(RANK, RANK);
	PHI.resize(RANK, RANK);
	Q.resize(RANK, RANK);
	qc.resize(RANK, RANK);
	F.setZero();
	PHI.setZero();
	Q.setZero();
	qc.setZero();
	fb = Vector3d(m_tagImuData.fx, m_tagImuData.fy, m_tagImuData.fz);
	Reb = Euler2RotationMatrix(CurState.att);
	Rbe = Reb.transpose();
	fe = Rbe * fb;

	F.block<3, 3>(0, 3) = Matrix3d::Identity();
	F.block<3, 3>(3, 3) = -2.0 * Skew(we_ie);
	F.block<3, 3>(3, 6) = Skew(fe.data());
	F.block<3, 3>(3, 9) = Rbe;
	F.block<3, 3>(6, 6) = -Skew(we_ie);
	F.block<3, 3>(6, 12) = -Rbe;

	qc.block<3, 3>(3, 3) = Rbe * m_mIta_v * Rbe.transpose();
	qc.block<3, 3>(6, 6) = Rbe * m_mIta_a * Rbe.transpose();

	PHI = Matrix<double, RANK, RANK>::Identity() + F * m_ddt;
	Q = 0.5 * (PHI * qc * PHI.transpose() + qc) * m_ddt;

	//◊¥Ã¨‘§≤‚
	m_vdx = PHI * m_vdx;

	m_mP.block<9, 9>(0, 0) = PHI.block<9, 15>(0, 0) * m_mP * PHI.block<9, 15>(0, 0).transpose() + Q.block<9, 9>(0, 0);
	m_mP.block<3, 3>(9, 9) = m_mP.block<3, 3>(9, 9) + Q.block<3, 3>(9, 9);
	m_mP.block<3, 3>(12, 12) = m_mP.block<3, 3>(12, 12) + Q.block<3, 3>(12, 12);
}
void CInsLooseCoupling::GNSSUpdate(const NavState& CurState)
{
	Matrix3d Reb;
	Vector3d le, z;
	MatrixXd H, K, R, I;
	H.resize(3, RANK);
	R.resize(3, 3);
	H.setZero();
	I = MatrixXd::Identity(RANK, RANK);

	Reb = Euler2RotationMatrix(CurState.att);
	le = Reb.transpose() * Vector3d(m_dlever);
	z = Vector3d(CurState.pos) + le - Vector3d(m_tagGnssState.pos);

	H.block<3, 3>(0, 0) = Matrix3d::Identity();
	H.block<3, 3>(0, 6) = Skew(le.data());

	R = Vector3d(m_tagGnssCov.pos).asDiagonal();

	//¡ø≤‚∏¸–¬
	K = m_mP * H.transpose() * (H * m_mP * H.transpose() + R).inverse();
	m_vdx = m_vdx + K * (z - H * m_vdx);
	m_mP = (I - K * H) * m_mP * (I - K * H).transpose() + K * R * K.transpose();
	//std::cout << m_vdx;
}
void CInsLooseCoupling::CompensateIMU()
{
	m_tagImuData.fx -= m_dAccBias[0];
	m_tagImuData.fy -= m_dAccBias[1];
	m_tagImuData.fz -= m_dAccBias[2];

	m_tagImuData.wx -= m_dGyrBias[0];
	m_tagImuData.wy -= m_dGyrBias[1];
	m_tagImuData.wz -= m_dGyrBias[2];
}
void CInsLooseCoupling::FeedBack(NavState& CurState)
{
	CurState.time = m_tagImuData.time;
	CurState.pos[0] -= m_vdx(0);
	CurState.pos[1] -= m_vdx(1);
	CurState.pos[2] -= m_vdx(2);

	CurState.vel[0] -= m_vdx(3);
	CurState.vel[1] -= m_vdx(4);
	CurState.vel[2] -= m_vdx(5);

	Matrix3d Reb;
	Reb = Euler2RotationMatrix(CurState.att);
	Vector3d temp = m_vdx.block<3, 1>(6, 0);
	Matrix3d NewRbe = (Matrix3d::Identity() + Skew(temp.data())) * Reb.transpose();
	RotationMatrix2Euler(NewRbe.transpose(), CurState.att);

	m_dAccBias[0] -= m_vdx(9);
	m_dAccBias[1] -= m_vdx(10);
	m_dAccBias[2] -= m_vdx(11);

	m_dGyrBias[0] -= m_vdx(12);
	m_dGyrBias[1] -= m_vdx(13);
	m_dGyrBias[2] -= m_vdx(14);

	m_vdx.setZero();
}
IMUData CInsLooseCoupling::Interpolate(IMUData& ImuData, const GPSTime Time)
{
	IMUData imu_mid;
	imu_mid.time = Time;

	imu_mid.fx = ImuData.fx;
	imu_mid.fy = ImuData.fy;
	imu_mid.fz = ImuData.fz;

	imu_mid.wx = ImuData.wx;
	imu_mid.wy = ImuData.wy;
	imu_mid.wz = ImuData.wz;
	return imu_mid;
}