#pragma once
#include"Particle.h"

class ParticleMPM
	:public Particle
{
public:
	ParticleMPM();
	virtual~ParticleMPM();
	virtual Matrix2d EnergyDerivative() = 0;
	virtual void MPMUpdate() = 0;
	virtual void DeltaForce() = 0;//隐式积分用，以后可能要重构
	void Update() { MPMUpdate(); }
	bool volume_check;
	Matrix2d def_elastic;//弹性形变梯度
	Matrix2d def_plastic;//范性形变梯度
	Matrix2d svd_u, svd_v, svd_s;
	Matrix2d polar_r, polar_s;//极分解结果
	Matrix2d velocity_gradient;//速度梯度
	Matrix2d B;//APIC中的仿射矩阵
	Vector2d grid_position;
	Vector2d weight_gradient[16];
	double coh;//后期考虑用类模板进行重构
	double weights[16];//保存该点到周围16个格点的B样条权重值
	Matrix2d delta_v;
	Matrix2d delta_f;
};