#pragma once
#include<time.h>
#include <fstream>  
#include<sstream>
#include <list>
#include <Eigen/Dense>
#include<opencv2/opencv.hpp>
#include"Particle.h"
#include"../geometry/SDF.h"
#include"../math/Function.h"

using namespace std;
using namespace Eigen;

class ParticleCloud 
{
public:
	int size;
	int p_num;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	virtual~ParticleCloud();
};

