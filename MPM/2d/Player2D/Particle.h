#pragma once
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include<iostream>
#include"Parameters.h"
using namespace std;
using namespace Eigen;

class Particle 
{
public:
	float volume, mass, density;
	float J;
	Vector2f position, velocity;
	Matrix2f def_elastic;//形变梯度
	Matrix2f svd_u, svd_v, svd_s;
	Matrix2f polar_r, polar_s;//极分解结果
	float velocity_gradient;//速度梯度
	Matrix2f B;//APIC中的仿射矩阵
	Vector2f grid_position;
	Vector2f weight_gradient[16];
	Vector3d color;
	float weights[16];//保存该点到周围16个格点的B样条权重值
	Particle();
	Particle(Vector2f pos, Vector2f vel, float mass);
	~Particle();
};