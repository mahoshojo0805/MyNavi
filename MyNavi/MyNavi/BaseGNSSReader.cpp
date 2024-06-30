#include "pch.h"
#include "BaseGNSSReader.h"
bool BaseGNSSReader::Next(NavState& GnssState, NavState& GnssStd)
{
	bool _flag = false;//是否读取成功
	switch (m_FileFormat)
	{
	case POS:
		_flag = ReadASC(GnssState, GnssStd);
		break;
	default:
		_flag = false;
		break;
	}
	return _flag;
}
bool BaseGNSSReader::Openfile(const char* filepath)
{
	m_FileFormat = GetFormatFromFilepath(filepath);
	if (m_FileFormat == UNKNOWN)
		return false;
	if (m_FileFormat == POS)
		fopen_s(&fin, filepath, "r");
	if (fin == NULL)return false;
	return true;
}
void BaseGNSSReader::Closefile()
{
	if (fin != NULL)
		fclose(fin);
}
BaseGNSSReader::FileFormat BaseGNSSReader::GetFormatFromFilepath(string filepath)
{
	FileFormat out;
	int length = string(filepath).length();
	int offset = string(filepath).find_last_of('.');
	string format = string(filepath).substr(offset + 1, length - offset);//remove "." from the string
	if (!format.compare("POS") || !format.compare("pos"))
		out = POS;
	else
		out = UNKNOWN;
	return out;
}
bool BaseGNSSReader::EndOfFile()
{
	if (fin != NULL)
		return feof(fin);
	else return true;
}
void BaseGNSSReader::Rewind()
{
	fseek(fin, 0, SEEK_SET);
}
bool BaseGNSSReader::ReadASC(NavState& GnssState, NavState& GnssStd)
{
	if (feof(fin) != 0 || fin == NULL)
		return false;

	fscanf_s(fin, "%d%lf%lf%lf%lf%lf%lf%lf",
		&GnssState.time.week, &GnssState.time.sow,
		&GnssState.pos[0], &GnssState.pos[1], &GnssState.pos[2],
		&GnssStd.pos[0], &GnssStd.pos[1], &GnssStd.pos[2]);

	return true;
}
