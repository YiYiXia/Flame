#pragma once
#include <vector>
#include <math.h>
#include <Eigen/Dense>//Eigen，如果路径不同，需要更换路径
#include<iostream>
#include"Parameters.h"
using namespace std;
using namespace Eigen;
//此类为粒子类，包括粒子的属性以及其相关的操作
class Particle 
{
public:
	float volume, mass, density;//体积、质量、密度
	Vector3f position, velocity;//位置、速度
	Matrix3f def_elastic;//形变梯度
	Matrix3f svd_u, svd_v, svd_s;//存放SVD分解的结果
	Matrix3f polar_r, polar_s;//极分解结果
	Matrix3f velocity_gradient;//速度梯度
	Matrix3f B;//仿射矩阵
	Vector3f grid_position;//格点位置
	Vector3f weight_gradient[64];//用于存放粒子到周围64个格点的权重
	float weights[64];//用于存放该点到周围16个格点的B样条权重值
	Particle();
	Particle(Vector3f pos, Vector3f vel, float mass);//粒子构造函数
	~Particle();
	void updatePos();//更新粒子位置
	void updateGradient();//形变梯度的更新
	void applyPlasticity();//将形变梯度进行SVD分解
	const Matrix3f energyDerivative();//计算粒子受力时的关键项
};