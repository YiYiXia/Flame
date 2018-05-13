/*
日期：2017.12.16
作者：夏一鸣
内容：实现Fast Marching Method（FMM），并应用到最短路径中。
库：Eigen、OpenCV
*/

#pragma once
#include<iostream>
#include<math.h>
#include<iomanip>
#include<vector>
#include"Bitree.h"
#include"Eigen/Dense"
#include"opencv2/opencv.hpp"
using namespace std;
using namespace Eigen;

enum STATE
{
	DEAD,
	OPEN,
	FARER,
	BOUNDARY
};

struct  Point
{
	int i, j, n;
	float f;
	STATE s;
	Point* u;
	Point* d;
	Point* l;
	Point* r;

};

class FastMarching 
{
public:
	FastMarching(int _x,int _y,float _h);
	~FastMarching();
	void Caculatedistance();
	void AddStart(int start_x, int start_y, int end_x, int end_y);
	void AddBoundary(int start_x, int start_y, int end_x, int end_y);
	float Value(int i, int j);
	void Display();
	void Show();
	void SearchPath(int ix,int jx);
	int x, y;
	float dmax;
private:
	Point *points;
	vector<Point*> open;
	float dmin;
	int nmin;
	float h;
	int boundary;
	int num_open;
	cv::Mat record;
	void Sort();
	void Delet(int n);
	float Functionx(int i, int j);
	void Distance(Point &p);
	void Initial();
	void Image();
};