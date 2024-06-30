/**
 * @file BaseIMUReader.h
 * @brief IMU数据读取器类
 * @details 读取IMU数据
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
	 * @brief					打开指定的文件
	 * @param filepath			[in]文件路径
	 * @param format			[in]指定打开的格式
	 * @return					true-文件打开成功；false-文件不存在
	 * @note
	 */
	bool Openfile(const char* filepath);
	/**
	 * @brief					关闭当前打开的文件
	 * @note
	 */
	void Closefile();
	/**
	 * @brief					下一条数据
	 * @param imudata			[out]输出的imu数据
	 * @return					true-读取成功；false-读取失败，文件到达末尾
	 * @note
	 */
	bool Next(IMUData& imudata);
	/**
	 * @brief					将文件流指针重置到开头
	 */
	void Rewind();
	/**
	 * @brief					是否到达文件结尾
	 * @return					true-是；false-否
	 * @note
	 */
	bool EndOfFile();
private:
	/**
	 * @brief					从文件路径中提取出文件类型
	 * @param filepath			文件路径
	 * @return					文件类型结构体
	 * @note
	 */
	static FileFormat GetFormatFromFilepath(string filepath);
	/**
	 * @brief					按照ASC格式读取
	 * @param imudata			输出的imudata
	 * @return					true-读取成功；false-读取失败，文件到达末尾
	 * @note
	 */
	bool ReadASC(IMUData& imudata);
};

