#pragma once
#include"ParticleCloud.h"
#include"Parameters.h"
#include <fstream>  
#include<sstream>
#include <direct.h>  
#include<string.h>
#include<Windows.h> 
#include <time.h>
#include<string>
#define MAXPATH  1024  
using namespace std;

class File
{
public:
	File();
	~File();
	string getTime();
	void FileOut(int frame, ParticleCloud cloud);
	void FileIn(int frame, ParticleCloud &cloud);
	void FileIn(int frame, vector<Vector2d> &pointlist);
	void ParametersIn();
	void Initial();
	char buffer[MAXPATH];
private:
	
};