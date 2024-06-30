#pragma once
#include<string>
#define MAX_BUFFER 1024
using namespace std;
class CConverter
{
public:
	FILE* fin;
	FILE* fout;

	void ConvertIMR(string filepath, string folderpath,string* header_info);
	void ConvertBIN(string filepath, string folderpath, string* info);
};

