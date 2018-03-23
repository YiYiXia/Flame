#include "Grid.h"
#include <iostream>

Grid::Grid()
{
}

Grid::Grid(int n, double delta_x)
{
	rows = n;
	cols = n;
	dx = delta_x;
	// allocate space for elem
	elem = new vector<int> *[rows];
	for (int i = 0; i < rows; i++)
		elem[i] = new vector<int>[cols];
};

Grid::~Grid()
{
}

void Grid::countParticles(vector<Particle> particles)
{
	int i, grid_i, grid_j;
	for (grid_i = 0; grid_i < rows; grid_i++)
		for (grid_j = 0; grid_j < cols; grid_j++)
			elem[grid_i][grid_j].clear();
	for (i = 0; i < particles.size(); i++)
	{
		grid_i = floor(particles[i].x(0) / dx);
		grid_j = floor(particles[i].x(1) / dx);
		if (grid_i >= 0 && grid_i <= rows && grid_j >0 && grid_j < cols)
			elem[grid_i][grid_j].push_back(i);
	}
}

vector<int> Grid::findNeighbors(vector<Particle> particles, Vector2d pos, double radius)
{
	// locate grid node
	int i, j, k;
	vector<int> neighbors;

	for (i = max( int(floor((pos(0) - radius) / dx)), 0 ); i <= min( int(ceil((pos(0) + radius) / dx)), rows-1 ); i++)
		for (j = max( int(floor((pos(1) - radius) / dx)), 0 ); j <= min( int(ceil((pos(1)+ radius) / dx)), cols-1 ); j++)
			for (k = 0; k < elem[i][j].size(); k++)
				neighbors.push_back(elem[i][j][k]);
	return neighbors;
}