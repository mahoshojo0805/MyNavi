#include "pch.h"
#include "CConverter.h"
int I4(unsigned char* p) { int res = 0; memcpy(&res, p, 4); return res; }
unsigned short U2(unsigned char* p) { unsigned short res = 0; memcpy(&res, p, 2); return res; }
unsigned int U4(unsigned char* p) { unsigned int res = 0; memcpy(&res, p, 4); return res; }
float R4(unsigned char* p) { float res = 0; memcpy(&res, p, 4); return res; }
double R8(unsigned char* p) { double res = 0; memcpy(&res, p, 8); return res; }
void CConverter::ConvertIMR(string filepath, string folderpath, string* header_info)
{
	fopen_s(&fin, filepath.c_str(), "rb");

	static int l = 0;
	static char buffer[MAX_BUFFER];
	int r = fread_s(buffer, MAX_BUFFER - l, sizeof(char), MAX_BUFFER - l, fin);
 	l += r;

	//Header
	char NULL_TERMINATED[8];
	int bDeltaTheta, bDeltaVelocity;
	double dDataRateHZ;
	double dGyroScaleFactor, dAcclScaleFactor;
	int iUtcOrGpsTime;
	int iWeek;
	double dSoW;

	memcpy(NULL_TERMINATED, buffer, 8);
	if (strcmp(NULL_TERMINATED, "$IMURAW") != 0)
	{
		*header_info = "Format Error|格式错误";
		fclose(fin);
		fclose(fout);
		return;
	}
	fopen_s(&fout, (folderpath + "\\IMUFILE.ASC").c_str(), "w");

	dDataRateHZ = R8((unsigned char*)buffer + 25);
	dGyroScaleFactor = R8((unsigned char*)buffer + 33);
	dAcclScaleFactor = R8((unsigned char*)buffer + 41);
	iUtcOrGpsTime = I4((unsigned char*)buffer + 49);
	iWeek = I4((unsigned char*)buffer + 49 + 84);
	dSoW = R8((unsigned char*)buffer + 49 + 84 + 4);
	*header_info = "采样率：" + to_string(dDataRateHZ) +
		"\n陀螺比例因子倒数：" + to_string(1.0 / dGyroScaleFactor)
		+ "\n加速度计比例因子倒数：" + to_string(1.0 / dAcclScaleFactor);

	memcpy(buffer, buffer + 512, l - 512);
	l -= 512;

	//Body
	while (feof(fin)==0)
	{	
		int i = 0;
		r = fread_s(buffer, MAX_BUFFER - l, sizeof(char), MAX_BUFFER - l, fin);
  		l += r;
		while(i<l)
		{
			double time = R8((unsigned char*)buffer + i + 0);
			int gx, gy, gz, fx, fy, fz;
			gx = I4((unsigned char*)buffer + i + 8);
			gy = -I4((unsigned char*)buffer + i + 12);
			gz = I4((unsigned char*)buffer + i + 16);
			fx = I4((unsigned char*)buffer + i + 20);
			fy = -I4((unsigned char*)buffer + i + 24);
			fz = I4((unsigned char*)buffer + i + 28);
			fprintf(fout, "%cIMURAWSA,%d,%.3lf;%d,%lf,00000077,%d,%d,%d,%d,%d,%d\n",'%', 2315, time, 2315, time, fz, fy, fx, gz, gy, gx);
			if (i == l)
				int pause = 0;
			if (i + 32 > l)
				break;
			i += 32;
		}
		memcpy(buffer, buffer + i, MAX_BUFFER - i);
		l = MAX_BUFFER - i;
	}

	fclose(fin);
	fclose(fout);
}
void CConverter::ConvertBIN(string filepath, string folderpath, string* info)
{
	fopen_s(&fin, filepath.c_str(), "rb");
	fopen_s(&fout, (folderpath + "\\IMUFILE.ASC").c_str(), "w");
	static int l = 0, r;
	static unsigned char buffer[MAX_BUFFER];
	const unsigned char synch[3] = { 0xAA,0x44,0x13 };
	double factor_A = 100.0 / 9.806 / 655360;
	double factor_G = 100.0 / 160849.543863;
	
	while (!feof(fin)) {
		r = fread_s(buffer + l, MAX_BUFFER - l, sizeof(char), MAX_BUFFER - l, fin);
		l += r;
		int i = 0;
		while (i < l - 12)
		{
			//HEADER
			if (buffer[i] != synch[0] || buffer[i + 1] != synch[1] || buffer[i + 2] != synch[2]) {
				i++;
				continue;
			}//同步符
			unsigned int Message_Length = *(buffer + i + 3);
			unsigned short GPSweek = U2(buffer + i + 6);
			//int id = U2(buffer + i + 4);
			double GPSsow = 1e-3 * I4(buffer + i + 8);//ms to s
			int H = 12;
			//BODY
			if (i + H + Message_Length + 5 < l) {
				unsigned long IMUweek = U4(buffer + i + H + 0);
				double IMUsow = R8(buffer + i + H + 4);
				long fz = I4(buffer + i + H + 16);
				long fy = -I4(buffer + i + H + 20);
				long fx = I4(buffer + i + H + 24);
				//double testfz = (double)(fz)*factor_A;
				//double testfy = (double)(fy)*factor_A;
				//double testfx = (double)(fx)*factor_A;
				long gz = I4(buffer + i + H + 28);
				long gy = -I4(buffer + i + H + 32);
				long gx = I4(buffer + i + H + 36);
				//double testgz = (double)(gz)*factor_G;
				//double testgy = (double)(gy)*factor_G;
				//double testgx = (double)(gx)*factor_G;
				//double test = 0;
				fprintf(fout, "%cRAWIMUSA,%hd,%lf;%hd,%lf,00000077,%ld,%ld,%ld,%ld,%ld,%ld\n", '%', GPSweek, GPSsow, IMUweek, IMUsow, fz, fy, fx, gz, gy, gx);
			}
			else {
				//i++;
				break;
			}
			i++;
		}
		memcpy(buffer, buffer + i, MAX_BUFFER - i);
		l = MAX_BUFFER - i;
	}

	fclose(fin);
	fclose(fout);
}
