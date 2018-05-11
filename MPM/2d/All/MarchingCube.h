#pragma once
#include<map>
#include<list>
#include<Eigen/Dense>
#include<opencv2/opencv.hpp>
#include<vector>
#include"SDF.h"
#include"Parameters.h"
using namespace std;
using namespace Eigen;
typedef struct Point
{
	Vector2d c;
	Point* p1;
	Point* p2;
	int index;
	Point(Vector2d e)
	{
		c = e;
		p1 = NULL;
		p2 = NULL;
		index = -1;
	}
}Point;

class MarchingCube
{
public:
	MarchingCube();
	~MarchingCube();
	vector<SDF> polygons;
	map<int, Point*> judge;
	map<int, Point*> check;
	void Polygon(cv::Mat p);
};
