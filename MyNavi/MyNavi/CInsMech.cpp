#include "pch.h"
#include "CInsMech.h"
void CInsMech::UpdateAtt(NavState* curstate, const NavState* laststate, const IMUData* imu_k, const double dt)
{
	Matrix3d Rbb, Ree, Reb;
	Quaterniond qee, qbb, qeb, q;
	//k-1ʱ��eϵ��bϵ����ת
	Reb = Euler2RotationMatrix(laststate->att);
	qeb = Quaterniond(Reb);
	//k-1ʱ��bϵ��kʱ��bϵ����ת
	Rbb = Euler2RotationMatrix(imu_k->wz * dt, imu_k->wy * dt, imu_k->wx * dt);
	qbb = Quaterniond(Rbb);
	//kʱ��eϵ��k-1ʱ��eϵ����ת
	Ree = Euler2RotationMatrix(-OMEGA_EARTH * dt, 0.0, 0.0);
	qee = Quaterniond(Ree);
	//�ϳɴ����ϣ��õ�kʱ��eϵ��bϵ����ת
	q = qbb * qeb * qee;
	//Matrix3d test = q.toRotationMatrix();
	RotationMatrix2Euler(q.toRotationMatrix(), curstate->att);
}
void CInsMech::UpdateVel(NavState* curstate, const NavState* laststate, const IMUData* imu_k, const double* ge, const double dt)
{
	Matrix3d Reb;//eϵ��bϵ��ת����
	Vector3d delta_v, delta_theta;//���ٶȼ���������������
	Vector3d we_ie, wb_ie, delta_thetav;//eϵ�µ�����ת���ٶȡ�bϵ�µ�����ת���ٶȡ�����������תӰ���ĽǶ�����
	Vector3d deltave_SF, deltave_G;//�������������������
	Vector3d V;//��ǰ�ٶ�

	Reb = Euler2RotationMatrix(curstate->att);
	delta_v = Vector3d(imu_k->fx * dt, imu_k->fy * dt, imu_k->fz * dt);
	delta_theta = Vector3d(imu_k->wx * dt, imu_k->wy * dt, imu_k->wz * dt);
	we_ie = Vector3d(0, 0, OMEGA_EARTH);

	wb_ie = Reb * we_ie;
	delta_thetav = delta_theta - wb_ie * dt;//wb_ib-wb_ie=wb_eb
	deltave_SF = Reb.transpose() * (delta_v + 0.5 * delta_thetav.cross(delta_v));
	deltave_G = (Vector3d(ge) - 2.0 * we_ie.cross(Vector3d(laststate->vel))) * dt;
	V = Vector3d(laststate->vel) + deltave_SF + deltave_G;
	memcpy(curstate->vel, V.data(), sizeof(double) * 3);
}
void CInsMech::UpdatePos(NavState* curstate, const NavState* laststate, const double dt)
{
	Vector3d Vk, Vk1;//��ǰʱ���ٶȡ���һʱ���ٶ�
	Vector3d curPos, lastPos;//��ǰλ�á���һʱ��λ��
	Vk = Vector3d(curstate->vel);
	Vk1 = Vector3d(laststate->vel);
	lastPos = Vector3d(laststate->pos);
	curPos = lastPos + 0.5 * (Vk + Vk1) * dt;
	memcpy(curstate->pos, curPos.data(), sizeof(double) * 3);
}