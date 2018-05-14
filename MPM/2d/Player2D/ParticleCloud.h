#pragma once
#include"Particle.h"
#include <GL/freeglut.h>
using namespace std;
using namespace Eigen;
class ParticleCloud 
{
public:
	
	vector<Particle> particles;
	int size;
	int frame;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	~ParticleCloud();
	void merge(const ParticleCloud& other);
	static ParticleCloud* Insert(std::vector<Particle> points)
	{
		ParticleCloud *obj = new ParticleCloud(points.size());

		for (int i = 0; i < points.size(); i++)
		{
			obj->particles.push_back(points[i]);
		}
		return obj;
	}
	void draw();
	void output();
	void fileout();
	void filein(int n);
};