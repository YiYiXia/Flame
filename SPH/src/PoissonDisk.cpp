#include <iostream>
#include <vector>
#include <ctime>
#include "PoissonDisk.h"
#include "Parameters.h"

PoissonDisk::PoissonDisk() {
}

PoissonDisk::~PoissonDisk() {
}

bool rejectedByNeighbors(Vector2d pos, vector<Vector2d> particles, int cell_num, Grid grid, double min_distance)
{
	int ref_i = floor(pos(0) * cell_num);
	int ref_j = floor(pos(1) * cell_num);
	if (ref_i < 0 || ref_i >= cell_num || ref_j < 0 || ref_j >= cell_num || grid.elem[ref_i][ref_j].size() > 0)
		return true;
	for (int i = ref_i - 2; i <= ref_i + 2; i++)
		for (int j = ref_j - 2; j <= ref_j + 2; j++)
			if (i >= 0 && i < cell_num && j >= 0 && j < cell_num && grid.elem[i][j].size() > 0)
			{
				Vector2d neigh_x = particles[grid.elem[i][j][0]];
				double dis = (neigh_x - pos).norm();
				if (dis < min_distance)
					return true;
			}
	return false;
}

vector<Vector2d> PoissonDisk::sample( int trial_num, double min_distance) {
    srand(time(NULL));
	vector<Vector2d> sample_particles;

    /* Step 0: initialize grid and active list */
    int cell_num = ceil(sqrt(2)/min_distance);
    double width = 1/double(cell_num);
	grid = Grid(cell_num, width);

    /* Step 1: select initial sample */
    Vector2d first_x;
    first_x << rand()/(double)RAND_MAX, rand()/(double)RAND_MAX;
    sample_particles.push_back(first_x);
    int i = floor(first_x(0)/width);
    int j = floor(first_x(1)/width);
    grid.elem[i][j].push_back(0);
    active_list.push_back(0);

    /* Step 2: sample more particles */
    while( !active_list.empty() ) {
        // randomly choose an index
        int p = rand() % active_list.size();
        int index = active_list[p];
        Vector2d prev_x = sample_particles[index];
        bool flag = false;
		for (int k = 1; k <= trial_num; k++) {
			// choose particle in radical annulus
			Vector2d new_x;
			double r = min_distance + min_distance * rand() / (double)RAND_MAX;
			double theta = 2 * Pi * rand() / (double)RAND_MAX;
			new_x << prev_x(0) + r * cos(theta), prev_x(1) + r * sin(theta);
			// identify grid node
			int new_i = floor(new_x(0) / width);
			int new_j = floor(new_x(1) / width);
            // rejection from neighbor particles
			if (rejectedByNeighbors(new_x, sample_particles, cell_num, grid, min_distance))
				continue;
            // sample
            sample_particles.push_back(new_x);
            int new_index = sample_particles.size() - 1;
            grid.elem[new_i][new_j].push_back(new_index);
            active_list.push_back(new_index);
            flag = true;
        }
        // remove p from active list
        if(!flag)
            active_list.erase(active_list.begin()+p);
    }

	return sample_particles;
}