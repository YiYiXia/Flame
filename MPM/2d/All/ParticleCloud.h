#pragma once
#include"Particle.h"
#include"ParticleSand.h"
#include"ParticleWater.h"
#include"ParticleElastic.h"
#include"Parameters.h"
#include <GL/freeglut.h>
#include<time.h>
#include"SDF.h"
#include <Eigen/Dense>
#include <list>
using namespace std;
using namespace Eigen;
enum ParticleType
{
	SAND,
	WATER,
	SNOW,
	ELASTIC
};

class ParticleCloud 
{
public:
	
	vector<Particle*> particles;
	int size;
	int p_num;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	~ParticleCloud();

	void AddParticles(ParticleType type, SDF* polygon);
	void InitialSample();
	void update();
	void merge(ParticleCloud& other);
	void copy(ParticleCloud other);

	ParticleCloud* Insert(std::vector<Particle*> points);
	
	void draw();
	void output();
private:
	double random_number(double lo, double hi);
	vector<Vector2d> PossionDisc(double r, SDF* polygon, int k);
	vector<Particle*> PossionSample(ParticleType type, SDF* polygon, Vector2d v, double r, int k);
	vector<Particle*> OrderSample(Vector2d start, Vector2d end, Vector2d size, Vector2d v);
};