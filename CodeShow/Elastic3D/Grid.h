#pragma once
#include <vector>
#include <math.h>
#include <Eigen/Dense>//Eigen，如果路径不同，需要更换路径
#include<iostream>
#include"ParticleCloud.h"
using namespace std;
using namespace Eigen;
const float BSPLINE_EPSILON = 1e-6;
const int BSPLINE_RADIUS = 2;

typedef struct GridNode//每个格点的属性 
{
	float mass;//格点质量
	bool active;//该格点在计算时是否应该被考虑
	Vector3f velocity, velocity_new;//格点速度
	Vector3f position;//格点所在位置
} GridNode;

//格点类：此类用于格点的操作。
class Grid 
{
public:
	Vector3f start, end, size, cellsize;//格点的起始位置，终止位置，大小，每格的大小
	int nodes_length;//格点总数
	float node_area;//格点的面积
	GridNode* nodes;//存放所有格点
	ParticleCloud* obj;//粒子团类，用于格点粒子之间的信息交互
	Grid(Vector3f pos, Vector3f dims, Vector3f cells, ParticleCloud* object);
	~Grid();

	void initializeMass();//将质量从粒子插值到格点
	void initializeVelocities();//将速度从粒子插值到格点
	void calculateVolumes() const;//计算粒子的体积
	void explicitVelocities(const Vector3f& gravity);//显式：计算格点受力，更新格点的速度
	void updateVelocities() const;//更新粒子的速度，速度梯度，仿射矩阵
	void collisionGrid();//基于格点角度的碰撞检测，较为粗糙
	void collisionParticles() const;//基于粒子的碰撞检测，目前已废，弃之不用

	//插值样条
	static float bspline(float x)
	{
		x = fabs(x);
		float w;
		if (x < 1)
			w = x*x*(x / 2 - 1) + 2.0 / 3.0;
		else if (x < 2)
			w = x*(x*(-x / 6 + 1) - 2) + 4.0 / 3.0;
		else return 0;
		//Clamp between 0 and 1... if needed
		if (w < BSPLINE_EPSILON) return 0;
		return w;
	}

	//对B样条求导
	static float bsplineSlope(float x)
	{
		float abs_x = fabs(x), w;
		if (abs_x < 1)
			return 1.5*x*abs_x - 2 * x;
		else if (x < 2)
			return -x*abs_x / 2 + 2 * x - 2 * x / abs_x;
		else return 0;
	}
};