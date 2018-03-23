#pragma once
#include <vector>
#include "Particle.h"

using namespace std;

class Grid
{
private:
	int rows;
	int cols;
	double dx;

public:
	vector<int> **elem;

public:
	Grid();
	Grid(int n, double delta_x);
	~Grid();
	void countParticles(vector<Particle> particles);							// count how many particles are inside each grid node
	vector<int> findNeighbors(vector<Particle> particles, Vector2d pos, double radius);			// find neighbor particles within a radius
};