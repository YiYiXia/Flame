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
	bool volume_check;
	double volume, mass, density;
	double alpha;
	double delta_q;
	double q;
	double V_c;
	double Lambda;
	double Mu;
	int index;
	Vector2d position, velocity;
	Matrix2d def_elastic;//弹性形变梯度
	Matrix2d def_plastic;//范性形变梯度
	Matrix2d svd_u, svd_v, svd_s;
	Matrix2d polar_r, polar_s;//极分解结果
	Matrix2d velocity_gradient;//速度梯度
	Matrix2d B;//APIC中的仿射矩阵
	Vector2d grid_position;
	Vector2d weight_gradient[16];
	double weights[16];//保存该点到周围16个格点的B样条权重值
	Particle();
	Particle(Vector2d pos, Vector2d vel, double mass, int num = 0, double lambda = 0,double mu = 0);
	~Particle();
	void updatePos();
	void updateGradient();
	void applyPlasticity();
	const Matrix2d energyDerivative();
	Matrix2d Derivative(Matrix2d s);
	Matrix2d project();
};