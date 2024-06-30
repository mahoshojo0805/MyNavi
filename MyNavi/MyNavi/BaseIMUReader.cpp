#include "pch.h"
#include "BaseIMUReader.h"
bool BaseIMUReader::Next(IMUData& imudata)
{
	bool _flag = false;//是否读取成功
	switch (m_FileFormat)
	{
	case ASC:
		_flag = ReadASC(imudata); 
		break;
	default:
		_flag = false;
		break;
	}
	return _flag;
}
bool BaseIMUReader::Openfile(const char* filepath)
{
	m_FileFormat = GetFormatFromFilepath(filepath);
	if (m_FileFormat == UNKNOWN)
		return false;
	if (m_FileFormat == ASC)
		fopen_s(&fin, filepath, "r");
	if (fin == NULL)return false;
	return true;
}
void BaseIMUReader::Closefile()
{
	if (fin != NULL)
		fclose(fin);
}
BaseIMUReader::FileFormat BaseIMUReader::GetFormatFromFilepath(string filepath)
{
	FileFormat out;
	int length = string(filepath).length();
	int offset = string(filepath).find_last_of('.');
	string format = string(filepath).substr(offset + 1, length - offset);//remove "." from the string
	if (!format.compare("ASC") || !format.compare("asc"))
		out = ASC;
	else
		out = UNKNOWN;
	return out;
}
bool BaseIMUReader::EndOfFile()
{
	if (fin != NULL)
		return feof(fin);
	else return true;
}
void BaseIMUReader::Rewind()
{
	fseek(fin, 0, SEEK_SET);
}
bool BaseIMUReader::ReadASC(IMUData& imudata)
{
	if (feof(fin) != 0 || fin == NULL)
		return false;

	char buf[BUFFER_LENGTH];
	fgets(buf, BUFFER_LENGTH, fin);

	string line;
	line = buf;

	string head, body;
	head = line.substr(0, line.find(';', 0));
	body = line.substr(line.find(';', 0) + 1, line.size());

	string str;
	int l, r;
	//IMU WEEK
	l = 0;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.time.week = atoi(str.c_str());
	//IMU SOW
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.time.sow = atof(str.c_str());
	imudata.time.sow = atof(str.c_str());
	//?????
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	//fz
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.fz = atof(str.c_str()) * ACCSCALE * IMUFREQ;
	//-fy
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.fy = -atof(str.c_str()) * ACCSCALE * IMUFREQ;
	//fx
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.fx = atof(str.c_str()) * ACCSCALE * IMUFREQ;
	//wz 
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.wz = deg2rad(atof(str.c_str()) * GYOSCALE * IMUFREQ);
	//-wy
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.wy = -deg2rad(atof(str.c_str()) * GYOSCALE * IMUFREQ);
	//wx
	l = r + 1;
	r = body.find(',', l);
	str = body.substr(l, r - l);
	imudata.wx = deg2rad(atof(str.c_str()) * GYOSCALE * IMUFREQ);

	return true;
}
