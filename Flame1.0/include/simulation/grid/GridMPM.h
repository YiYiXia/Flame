#pragma once
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include<iostream>
#include <fstream>  
#include<sstream>
#include"../../geometry/SDF.h"
#include"../../physics/ParticleCloudMPM.h"
#include"../../math/Function.h"
using namespace std;
using namespace Eigen;
const double BSPLINE_EPSILON = 1e-64;
const int BSPLINE_RADIUS = 2;
enum MPMGridNodeType
{
	BOUNDARY,
	EMPTY,
	LIQUID
};

//可能有冗余，后面想办法减少
typedef struct MPMGridNode {
	double mass;
	double pressure;
	MPMGridNodeType type;
	bool active;
	bool collision;
	Vector2d velocity, velocity_new, velocity_out;
	Vector2d force;
	Vector2d x, r, r_new, p, Ls;//共轭梯度法所用的中间变量
	Vector2d Im_force;//隐式力
	Vector2d position;
	Vector2d normal;
	bool imp_active;
	int index;//格点的编号
	int s_index;
	MPMGridNode()
	{
		collision = false;
		normal = Vector2d::Identity();
	}
} MPMGridNode;

class GridMPM
{
public:
	Vector2d start, end, size, cellsize;
	double Time;
	double Friction_c;//摩擦系数，这一项暂放在这
	vector<SDF*> polygon;//交互边界，物体
	int nodes_length;//格点总数
	int active_length;//记录稀疏矩阵大小
	vector<MPMGridNode*> active_list;//存放待求解的格点
	double node_area;//格点的面积
	MPMGridNode* nodes;
	ParticleCloudMPM* obj;
	GridMPM(Vector2d pos, Vector2d dims, Vector2d cells, ParticleCloudMPM* object);
	GridMPM(Vector2d pos, Vector2d dims,  ParticleCloudMPM* object);
	~GridMPM();

	void InitializeMass();
	void InitializeVelocities();
	void CalculateVolumes() const;
	void ExplicitForce();
	void GridVelocityUpdate();
	void UpdateVelocities();
	void CollisionGrid();//处理格点碰撞
	void Friction();//计算格点摩擦力
	void ImplicitVelocities();
	void RecomputeImplicitForces();
	void ApplyExternForce();
	void ParametersIn();//读入网格的参数
	void ParticleCheck();//对越界粒子进行清理
	void GridCheck();//方程式解法的初始化
	void SolvePressure();//求解
	void SolveUpdate();
};