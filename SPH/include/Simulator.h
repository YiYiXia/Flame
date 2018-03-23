#pragma once
#include <Eigen>
#include <vector>
#include "Particle.h"
#include "Grid.h"

class Simulator
{
private:
	vector<Particle>	particles;				// particles system
	Grid				grid;					// background grid (for neighbor search)
	double				dt;						// time step length
	double				frame_dt;				// frame dt length
	double				dx;						// grid dx width
	double				rho0;					// initial density
	double				k;						// stiffness parameter
	double				gamma;					// stretch parameter
	Vector2d			gravity;				// gravity
	double				t;						// time

private:
	void calDensity();					// calculate particle density
	void calPressure();					// calculate particle pressure
	void applyForces();					// apply forces on particles
	void handleCollisions();			// handle collisions with boundary
	void writeData(int frame_num);		// write particles data into file
	void renderScene();					// render scene on the screen

public:
	Simulator();
	~Simulator();
	void init(double delta_t, int res, double density, double k0, double gamma0, int frame_rate, Vector2d center, double scale);		// initialize parameters and sample particles
	void oneTimeStep();					// execute a whole cycle in one time step
	double getTime();					// return current time
};