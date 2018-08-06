#pragma once
#include <Eigen/Dense>
#include<vector>
#include"../../common/Parameters.h"
#include"../../physics/ParticleCloudMPM.h"

using namespace std;
using namespace Eigen;

enum MACNodeType
{
	FLUID,
	SOLID,
	MACNONE
};
//
typedef struct MACGridNode
{
	double pressure;
	double mass;
	double density;
	double liquid_phi;//记录粒子到中心的最短距离
	Vector2d velocity;
	Vector2d position;
	MACNodeType type;
	vector<ParticleMPM*> particles;//存放格点中的粒子
	int index;//格点的编号
	int index_x, index_y;
	int s_index;
	MACGridNode()
	{
		mass = 0;
	}
}MACGridNode;

class GridMAC
{
public:
	GridMAC(Vector2d pos, Vector2d dims, Vector2d cells);
	GridMAC(Vector2d pos, Vector2d dims, Vector2d cells, ParticleCloudMPM* object);
	~GridMAC();
	MACGridNode* nodes;
	vector<SDF*> polygon;//交互边界，物体
	Vector2d start, end, size, cellsize;//start是格点中心的起始位置、
	int nodes_length;//格点总数
	double *velocity_u;//交错辅助计算网格
	double *velocity_v;
	double *u_weights;
	double *v_weights;
	double *nodal_solid_phi;//记录从格点到边界的距离
	ParticleCloudMPM* obj;//粒子存放
	vector<MACGridNode*> active_list;//存放待求解的格点
	int active_length;
	double delta_t;
	void AddObject();
	void AddForce();
	//void CalculateCenter();
	void CalculateNodePhi();
	void ComputerPhi();
	void ComputerWeight();
	void ComputerNeighbour();
	void Correct();
	void GridCheck();
	void GetNeighbour(int i, int j, int wl, int wh, int hl, int hh, vector<ParticleMPM*> &neighbour);
	void Initial();
	void SolvePressure();
	//void Transport();
	//void Update();
	void UpdateParticle();
	void ParticleToGrid(); 
	void GridToParticle();
	void CollsionHandle();
	Matrix2d ComputerAffineMatrix(Vector2d p);
	//void SolvePressure();
};