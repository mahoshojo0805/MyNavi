/**
 * @file BaseGNSSReader.h
 * @brief GNSS数据读取器类
 * @details 读取GNSS数据
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
	 * @brief					打开文件
	 * @param filepath			文件路径
	 * @return					true-打开成功；false-打开失败
	 * @note
	 */
	bool Openfile(const char* filepath);
	/**
	 * @brief					关闭当前打开的文件
	 * @note
	 */
	void Closefile();
	/**
	 * @brief					读取下一条数据
	 * @param gnss_state		读取的GNSS数据
	 * @param gnss_std			读取的GNSS协方差
	 * @return					true-读取成功；false-读取失败
	 * @note
	 */
	bool Next(NavState& GnssState, NavState& GnssStd);
	/**
	 * @brief					将文件流指针重置到开头
	 * @note
	 */
	void Rewind();
	/**
	 * @brief					文件是否到达结尾
	 * @return					true-已经到达结尾；false-未到达结尾
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
	 * @brief					读取ASC格式的文件
	 * @param gnss_state		读取的GNSS数据
	 * @param gnss_std			读取的GNSS协方差
	 * @return					true-读取成功；false-读取失败，文件到达末尾
	 * @note
	 */
	bool ReadASC(NavState& GnssState, NavState& GnssStd);
};

