#pragma once
#include <vector>
#include <math.h>
#include <GL/freeglut.h>
#include <Eigen/Dense>
#include<iostream>
#include"ParticleCloud.h"
using namespace std;
using namespace Eigen;
const float BSPLINE_EPSILON = 1e-4;
const int BSPLINE_RADIUS = 2;

typedef struct GridNode {
	float mass;
	bool active;
	Vector2f velocity, velocity_new;
	Vector2f position;
} GridNode;

class Grid 
{
public:
	Vector2f start, end, size, cellsize;
	int nodes_length;//格点总数
	float node_area;//格点的面积
	GridNode* nodes;
	ParticleCloud* obj;
	Grid(Vector2f pos, Vector2f dims, Vector2f cells, ParticleCloud* object);
	~Grid();

	void initializeMass();
	void initializeVelocities();
	void calculateVolumes() const;
	void explicitVelocities(const Vector2f& gravity);
	void updateVelocities() const;
	void collisionGrid();
	void collisionParticles() const;
	void draw();
	void output();
	static float bspline(float x)
	{
		x = fabs(x);
		float w;
		if (x < 1)
			w = x*x*(x / 2 - 1) + 2 / 3.0;
		else if (x < 2)
			w = x*(x*(-x / 6 + 1) - 2) + 4 / 3.0;
		else return 0;
		//Clamp between 0 and 1... if needed
		if (w < BSPLINE_EPSILON) return 0;
		return w;
	}
	//Slope of interpolation function
	//对B样条求导
	static float bsplineSlope(float x)
	{
		float abs_x = fabs(x), w;
		if (abs_x < 1)
			return 1.5*x*abs_x - 2 * x;
		else if (x < 2)
			return -x*abs_x / 2 + 2 * x - 2 * x / abs_x;
		else return 0;
		//Clamp between -2/3 and 2/3... if needed
	}
};