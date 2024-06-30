/**
 * @file BaseGNSSReader.h
 * @brief GNSS���ݶ�ȡ����
 * @details ��ȡGNSS����
 * @author QYC
 * @version v1.0
 * @date 2024/6/28
 * @copyright QYC
 */
#pragma once
#include"BaseFunction.h"
#include"BaseStruct.h"
#include"Eigen/Dense"
#include<string>
#define BUFFER_LENGTH 1024
using namespace std;
class BaseGNSSReader
{
private:
	enum FileFormat { UNKNOWN, POS }m_FileFormat;
	FILE* fin;
public:
	/**
	 * @brief					���ļ�
	 * @param filepath			�ļ�·��
	 * @return					true-�򿪳ɹ���false-��ʧ��
	 * @note
	 */
	bool Openfile(const char* filepath);
	/**
	 * @brief					�رյ�ǰ�򿪵��ļ�
	 * @note
	 */
	void Closefile();
	/**
	 * @brief					��ȡ��һ������
	 * @param gnss_state		��ȡ��GNSS����
	 * @param gnss_std			��ȡ��GNSSЭ����
	 * @return					true-��ȡ�ɹ���false-��ȡʧ��
	 * @note
	 */
	bool Next(NavState& GnssState, NavState& GnssStd);
	/**
	 * @brief					���ļ���ָ�����õ���ͷ
	 * @note
	 */
	void Rewind();
	/**
	 * @brief					�ļ��Ƿ񵽴��β
	 * @return					true-�Ѿ������β��false-δ�����β
	 * @note
	 */
	bool EndOfFile();
private:
	/**
	* @brief					���ļ�·������ȡ���ļ�����
	* @param filepath			�ļ�·��
	* @return					�ļ����ͽṹ��
	* @note
	*/
	static FileFormat GetFormatFromFilepath(string filepath);
	/**
	 * @brief					��ȡASC��ʽ���ļ�
	 * @param gnss_state		��ȡ��GNSS����
	 * @param gnss_std			��ȡ��GNSSЭ����
	 * @return					true-��ȡ�ɹ���false-��ȡʧ�ܣ��ļ�����ĩβ
	 * @note
	 */
	bool ReadASC(NavState& GnssState, NavState& GnssStd);
};

