/**
 * @file BaseIMUReader.h
 * @brief IMU���ݶ�ȡ����
 * @details ��ȡIMU����
 * @author QYC
 * @version v
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
class BaseIMUReader
{
private:
	enum FileFormat{UNKNOWN,ASC}m_FileFormat;
	FILE* fin;
public:
	/**
	 * @brief					��ָ�����ļ�
	 * @param filepath			[in]�ļ�·��
	 * @param format			[in]ָ���򿪵ĸ�ʽ
	 * @return					true-�ļ��򿪳ɹ���false-�ļ�������
	 * @note
	 */
	bool Openfile(const char* filepath);
	/**
	 * @brief					�رյ�ǰ�򿪵��ļ�
	 * @note
	 */
	void Closefile();
	/**
	 * @brief					��һ������
	 * @param imudata			[out]�����imu����
	 * @return					true-��ȡ�ɹ���false-��ȡʧ�ܣ��ļ�����ĩβ
	 * @note
	 */
	bool Next(IMUData& imudata);
	/**
	 * @brief					���ļ���ָ�����õ���ͷ
	 */
	void Rewind();
	/**
	 * @brief					�Ƿ񵽴��ļ���β
	 * @return					true-�ǣ�false-��
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
	 * @brief					����ASC��ʽ��ȡ
	 * @param imudata			�����imudata
	 * @return					true-��ȡ�ɹ���false-��ȡʧ�ܣ��ļ�����ĩβ
	 * @note
	 */
	bool ReadASC(IMUData& imudata);
};

