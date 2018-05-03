#pragma once
#include"Particle.h"
#include"Parameters.h"
#include <GL/freeglut.h>
#include<time.h>
#include <Eigen/Dense>
#include <list>
using namespace std;
using namespace Eigen;
class ParticleCloud 
{
public:
	
	vector<Particle> particles;
	int size;
	int p_num;
	ParticleCloud();
	ParticleCloud(int cloud_size);
	~ParticleCloud();

	void InitialSample();
	void update();
	void merge(ParticleCloud& other);
	void copy(ParticleCloud other);
	ParticleCloud* Insert(std::vector<Particle> points);
	
	void draw();
	void output();
private:
	double random_number(double lo, double hi);
	vector<Vector2d> PossionDisc(double r, double start_x, double start_y, double x_length, double y_length, int k);
	vector<Particle> PossionSample(Vector2d start, Vector2d end, Vector2d v, double r, int k);
	vector<Particle> OrderSample(Vector2d start, Vector2d end, Vector2d size, Vector2d v);
};