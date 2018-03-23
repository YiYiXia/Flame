#include "Simulator.h"
#include "PoissonDisk.h"
#include "Parameters.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <GL/glut.h>

Simulator::Simulator()
{
}

Simulator::~Simulator()
{
}

double weightFunction(Vector2d x, double h)					// cubic spline weight function
{
	double r = x.norm();
	double q = r / h;
	double c = 15.0 / (7.0 * Pi * h * h);
	if (q < 1)
		return c * (1.0 / 2.0 * q * q * q - q * q + 2.0 / 3.0);
	else if (q < 2)
		return c * 1.0 / 6.0 * (2.0 - q) * (2.0 - q) * (2.0 - q);
	else
		return 0;
}

Vector2d weightGradient(Vector2d x, double h)				// cubic spline weight gradient
{
	double r = x.norm();
	double q = r / h;
	double c = 15.0 / (7.0 * Pi * h * h);
	if (q < 1)
		return c * (3.0 / 2.0 * q - 2.0) * x / (h * h);
	else if (q < 2)
		return -c * 1.0 / 2.0 * (2.0 - q) * (2.0 - q) * x / (r*h);
	else
		return Vector2d(0, 0);
}

void Simulator::init(double delta_t, int res, double density, double k0, double gamma0, int frame_rate, Vector2d center, double scale)
{
	t = 0;
	dt = delta_t;
	frame_dt = 1 / double(frame_rate);
	dx = 1 / double(res);
	rho0 = density;
	PoissonDisk pd;
	vector<Vector2d> particle_positions = pd.sample(30, dx / (3*scale));
	for (int i = 0; i < particle_positions.size(); i++)
	{
		double x = center(0) + (particle_positions[i](0) - 0.5) * scale;
		double y = center(1) + (particle_positions[i](1) - 0.5) * scale;
		particles.push_back(Particle(Vector2d(x,y), Vector2d(0, 0), rho0, density * dx * dx / 5));
	}
	grid = Grid(res, dx);
	grid.countParticles(particles);
	k = k0;
	gamma = gamma0;
	gravity = Vector2d(0, -9.8);
}

void Simulator::calDensity()
{
	vector<int> neighbors;
	double rho_i, w;
	for(int i = 0; i < particles.size(); i++)
	{
		rho_i = 0;
		neighbors = grid.findNeighbors(particles, particles[i].x, 2*dx);
		for (int j = 0; j < neighbors.size(); j++)
		{
			w = weightFunction(particles[neighbors[j]].x - particles[i].x, dx);
			rho_i += particles[neighbors[j]].m * w;
		}
		particles[i].rho = rho_i;
	}
}

void Simulator::calPressure()
{
	for (int i = 0; i < particles.size(); i++)
		particles[i].p = k * rho0 / gamma * ( pow( particles[i].rho / rho0, gamma) - 1 );
}

void Simulator::applyForces()
{
	vector<int> neighbors;
	Vector2d f_i, x_i, x_j, dw;
	double m_i, p_i, rho_i, p_j, rho_j;
	for (int i = 0; i < particles.size(); i++)
	{
		/* internal forces between particles */
		f_i = Vector2d(0, 0);
		x_i = particles[i].x;
		m_i = particles[i].m;
		p_i = particles[i].p;
		rho_i = particles[i].rho;
		neighbors = grid.findNeighbors(particles, particles[i].x, 2 * dx);
		for (int j = 0; j < neighbors.size(); j++)
		{
			x_j = particles[neighbors[j]].x;
			p_j = particles[neighbors[j]].p;
			rho_j = particles[neighbors[j]].rho;
			dw = weightGradient( x_j - x_i, dx);
			f_i += m_i * m_i * ( p_i/(rho_i*rho_i) + p_j/(rho_j*rho_j) ) * dw;
		}

		/* external forces: gravity */
		particles[i].v += (f_i/m_i + gravity) * dt;

		/* advect position */
		particles[i].x += particles[i].v * dt;
	}
}

void Simulator::handleCollisions()
{
	Vector2d normal;
	double vn;
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i].x(0) < 2 * dx)
		{
			normal = Vector2d(-1, 0);
			particles[i].x(0) = 2 * dx;
		}
		if (particles[i].x(0) > 1 - 2 * dx)
		{
			normal = Vector2d(1, 0);
			particles[i].x(0) = 1 - 2 * dx;
		}
		if (particles[i].x(1) < 2 * dx)
		{
			normal = Vector2d(0, -1);
			particles[i].x(1) = 2 * dx;
		}
		if (particles[i].x(1) > 1 - 2 * dx)
		{
			normal = Vector2d(0, 1);
			particles[i].x(1) = 1 - 2 * dx;
		}
		vn = particles[i].v.transpose() * normal;
		if (vn > 0)
			particles[i].v -= vn * normal;
	}
}

void Simulator::writeData(int frame_num)
{
	ofstream myfile; 
	ostringstream filename;
	filename << "../output/frame" << setfill('0') << setw(4) << frame_num << ".xy";
	myfile.open(filename.str());
	for (int i = 0; i < particles.size(); i++)
		myfile << fixed << setprecision(8) << particles[i].x(0) << " " << particles[i].x(1) << " " << endl;
	myfile.close();
}

void Simulator::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (int i = 0; i < particles.size(); i++)
		glVertex2f(WINDOW_WIDTH * particles[i].x(0), WINDOW_HEIGHT * particles[i].x(1));
	glEnd();
	glFlush();
}

void Simulator::oneTimeStep()
{
	int frame_num = floor(t / frame_dt);
	if (t - frame_num * frame_dt < dt)
	{
		writeData(frame_num);
		renderScene();
	}
	calDensity();
	calPressure();
	applyForces();
	handleCollisions();
	t = t + dt;
	grid.countParticles(particles);
}

double Simulator::getTime()
{
	return t;
}