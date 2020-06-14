#pragma once
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include<iostream>
#include"../common/Parameters.h"
using namespace std;
using namespace Eigen;

enum ParticleType
{
	WETSAND,
	DRYSAND,
	WATER,
	SNOW,
	ELASTIC,
	NONE
};
class Particle 
{
public:
	bool volume_check;
	double volume, mass, density;
	int index;
	Vector2d position, velocity;
	Vector3d color;
	ParticleType p_type;
	Particle();
	virtual~Particle();
	virtual void Update() = 0;

};