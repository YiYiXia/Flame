#pragma once
#include<time.h>
#include <fstream>  
#include<sstream>
#include <list>
#include <Eigen/Dense>
#include<opencv2/opencv.hpp>
#include"Particle.h"
#include"../geometry/SDF.h"
#include"../math/LinkList.h"
#include"../math/Function.h"

using namespace std;
using namespace Eigen;

typedef vector<Vector2d> PointList;
PointList OrderSample(SDF* polygon);
PointList PossionDisc(double r, SDF* polygon, int k);
PointList PossionDisc(double r, cv::Mat picture, SDF* polygon, int k);
bool MatContain(cv::Mat picture, double scalex, double scaley, Vector2d p);//判定点是否在图像内

class ParticleCloud 
{
public:
	int size;
	int p_num;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	virtual~ParticleCloud();
};

