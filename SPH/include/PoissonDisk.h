#pragma once
#include <vector>
#include "Grid.h"

using namespace std;

class PoissonDisk {
public:
    Grid grid;
    vector<int> active_list;

public:
    PoissonDisk();
    ~PoissonDisk();
	vector<Vector2d> sample( int trial_num, double min_distance);
};