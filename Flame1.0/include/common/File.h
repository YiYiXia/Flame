#pragma once
#include<fstream>  
#include<sstream>
#include<direct.h>  
#include<string.h>
#include<Windows.h> 
#include<time.h>
#include<string>
#include"../physics/ParticleCloud.h"
#include"../common/Parameters.h"

#define MAXPATH  1024  
using namespace std;

class File
{
public:
	File();
	~File();
	char buffer[MAXPATH];

	void CanvasOutput(int num, cv::Mat* canvas);//Output Canvas
	void CreateFolder();//Create a folder to save some files created by flame
	void FileIn(int frame, ParticleCloud &cloud);
	void FileOut(int frame, ParticleCloud cloud);
	
private:
	string getTime();//Get the time to name the folder
};