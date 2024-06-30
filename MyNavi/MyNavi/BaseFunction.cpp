#include "pch.h"
#include"BaseFunction.h"
double rad2deg(double a)
{
	return a * 180.0 / PI;
}
double deg2rad(double a)
{
	return a * PI / 180.0;
}
Matrix3d Euler2RotationMatrix(const double yaw, const double roll, const double pitch)
{
	Matrix3d R;
	double alpha = yaw;
	double beta = roll;
	double gama = pitch;
	R(0, 0) = cos(alpha) * cos(beta) - sin(alpha) * sin(beta) * sin(gama);
	R(1, 0) = -sin(alpha) * cos(gama);
	R(2, 0) = cos(alpha) * sin(beta) + sin(alpha) * cos(beta) * sin(gama);

	R(0, 1) = sin(alpha) * cos(beta) + cos(alpha) * sin(beta) * sin(gama);
	R(1, 1) = cos(alpha) * cos(gama);
	R(2, 1) = sin(alpha) * sin(beta) - cos(alpha) * cos(beta) * sin(gama);

	R(0, 2) = -sin(beta) * cos(gama);
	R(1, 2) = sin(gama);
	R(2, 2) = cos(beta) * cos(gama);
	return R;
}
Matrix3d Euler2RotationMatrix(const double* att)
{
	return Euler2RotationMatrix(att[0], att[1], att[2]);
}
void RotationMatrix2Euler(const double* R, double& yaw, double& roll, double& pitch)
{
	double R13 = R[2 * 3 + 0];
	double R21 = R[0 * 3 + 1];
	double R22 = R[1 * 3 + 1];
	double R23 = R[2 * 3 + 1];
	double R33 = R[2 * 3 + 2];
	yaw = atan2(-R21, R22);
	roll = atan2(-R13, R33);
	pitch = asin(R23);
}
void RotationMatrix2Euler(const double* R, double* att)
{
	RotationMatrix2Euler(R, att[0], att[1], att[2]);
}
void RotationMatrix2Euler(const Matrix3d& R, double& yaw, double& roll, double& pitch)
{
	yaw = atan2(-R(1, 0), R(1, 1));
	roll = atan2(-R(0, 2), R(2, 2));
	pitch = asin(R(1, 2));
}
void RotationMatrix2Euler(const Matrix3d& R, double* att)
{
	RotationMatrix2Euler(R, att[0], att[1], att[2]);
}
Matrix3d GetReL(const double xyz[],EPType ep_type)
{
	double blh[3];
	XYZ2BLH(xyz, blh, ep_type);
	return Euler2RotationMatrix(PI / 2 + blh[1], 0, PI / 2 - blh[0]);
}
void XYZ2BLH(double x, double y, double z, double& b, double& l, double& h, EPType ep_type)
{
	double xyz[3] = { x,y,z };
	double blh[3];
	XYZ2BLH(xyz, blh, ep_type);
	b = blh[0];
	l = blh[1];
	h = blh[2];
}
void XYZ2BLH(const double* xyz, double* blh, EPType ep_type)
{
	double e2;
	double a;
	double phi;
	double R;
	double W;
	double N;
	//选择合适的椭球参数
	switch (ep_type)
	{
	case WGS84:
		e2 = tagWGS84.e2; a = tagWGS84.a;
		break;
	case CGCS2000:
		e2 = tagCGCS2000.e2; a = tagCGCS2000.a;
		break;
	default:
		break;
	}
	//地心
	if (xyz[0] == 0 && xyz[1] == 0 && xyz[2] == 0)
	{
		blh[0] = 0;
		blh[1] = 0;
		blh[2] = -a;
		return;
	}

	phi = atan2(xyz[2], sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1]));
	R = sqrt(xyz[0] * xyz[0] + xyz[1] * xyz[1] + xyz[2] * xyz[2]);
	double B = 0;
	blh[0] = 0;
	do
	{
		B = blh[0];
		W = sqrt(1 - e2 * sin(B) * sin(B));
		blh[0] = atan(tan(phi) * (1 + a * e2 / xyz[2] * sin(B) / W));
	} while (blh[0] - B > 0.00000001);

	blh[1] = atan2(xyz[1], xyz[0]);

	W = sqrt(1 - e2 * sin(blh[0]) * sin(blh[0]));
	N = a / W;
	blh[2] = R * cos(phi) / cos(blh[0]) - N;
}
void BLH2XYZ(const double* blh, double* xyz, EPType ep_type)
{
	double e2;
	double N;

	switch (ep_type)
	{
	case WGS84:
		e2 = 2 * tagWGS84.f - tagWGS84.f * tagWGS84.f;
		N = tagWGS84.a / sqrt(1 - e2 * sin(blh[0]) * sin(blh[0]));
		break;
	case CGCS2000:
		e2 = 2 * tagCGCS2000.f - tagCGCS2000.f * tagCGCS2000.f;
		N = tagCGCS2000.a / sqrt(1 - e2 * sin(blh[0]) * sin(blh[0]));
		break;
	}

	xyz[0] = (N + blh[2]) * cos(blh[0]) * cos(blh[1]);
	xyz[1] = (N + blh[2]) * cos(blh[0]) * sin(blh[1]);
	xyz[2] = (N * (1 - e2) + blh[2]) * sin(blh[0]);
}
double GetGravity(const double b, const double h)
{
	const double g0 = 9.7803267715;
	const double a1 = 0.0052790414;
	const double a2 = 0.0000232718;
	const double b1 = -0.000003087691891;
	const double b2 = 0.0000000043977311;
	const double b3 = 0.0000000000007211;
	double sinb = sin(b);
	double sin2b = sinb * sinb;
	double sin4b = sin2b * sin2b;
	return g0 * (1.0 + a1 * sin2b + a2 * sin4b) + (b1 + b2 * sin2b) * h + b3 * h * h;
}
Matrix3d Skew(double* v)
{
	Eigen::Matrix3d skew;
	skew << 0, -v[2], v[1],
		v[2], 0, -v[0],
		-v[1], v[0], 0;
	return skew;
}
