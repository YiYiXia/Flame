#pragma once
#include"Particle.h"
using namespace std;
using namespace Eigen;
//此类用于粒子团的操作，
class ParticleCloud 
{
public:
	
	vector<Particle> particles;
	int size;
	int frame;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	~ParticleCloud();
	void update();//整个运算流程中，与粒子有关的流程
	void merge(const ParticleCloud& other);//粒子团的合并操作

	//粒子云的构造（此处是模仿的别人的代码）
	static ParticleCloud* Insert(std::vector<Particle> points)
	{
		ParticleCloud *obj = new ParticleCloud(points.size());

		for (int i = 0; i < points.size(); i++)
		{
			obj->particles.push_back(points[i]);
		}
		return obj;
	}
	void fileout();//将粒子团中的粒子信息用txt文件输出
};