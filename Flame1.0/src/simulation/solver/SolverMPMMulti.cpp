#include"../../../include/simulation/solver/SolverMPMMulti.h"

SolverMPMMulti::SolverMPMMulti()
{
}
SolverMPMMulti::~SolverMPMMulti()
{
}

void SolverMPMMulti::SolverInitial(IntegralType SType, vector<SDFInfo>* SDFlist)
{
	Type = SType;
	objList = SDFlist;
	water = new ParticleCloudMPM();
	grid_w = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), water);
	grid_w->InitializeMass();
	grid_w->CalculateVolumes();

	sand = new ParticleCloudMPM();
	grid_s = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), sand);
	grid_s->InitializeMass();
	grid_s->CalculateVolumes();

	for (int i = 0; i < (*objectList).size(); i++)
		if ((*objectList)[i]->appearTime == 0)
		{
			grid_w->objectList.push_back((*objectList)[i]);
			grid_s->objectList.push_back((*objectList)[i]);
		}
	for (int i = 0; i < (*boundaryList).size(); i++)
		if ((*boundaryList)[i]->appearTime == 0)
		{
			grid_w->boundaryList.push_back((*boundaryList)[i]);
			grid_s->boundaryList.push_back((*boundaryList)[i]);
		}
	cohesion_w = new double[grid_s->nodes_length];
}

void SolverMPMMulti::SolverUpdate()
{
	grid_s->InitializeMass();
	grid_s->InitializeVelocities();
	grid_s->ExplicitForce();
	grid_w->InitializeMass();
	grid_w->InitializeVelocities();
	grid_w->ExplicitForce();

	MomentaExchange();
	Saturation();
	grid_s->CollisionGrid();
	grid_s->Friction();
	grid_s->UpdateVelocities();
	sand->Update();
	grid_w->CollisionGrid();
	grid_w->Friction();
	grid_w->UpdateVelocities();
	water->Update();
	InformationReturn();
}
void SolverMPMMulti::AddParticle(vector<Vector2d> *particleList, ParticleType particleType, Vector2d particleVelocity, Vector3d particleColor)
{
	ParticleCloudMPM* temp;
	temp = new ParticleCloudMPM(*particleList, particleType, particleVelocity, particleColor);
	GridMPM*grid_temp;
	grid_temp = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
	grid_temp->InitializeMass();
	grid_temp->CalculateVolumes();
	if (particleType == WETSAND) sand->Merge(*grid_temp->obj);
	if (particleType == WATER) water->Merge(*grid_temp->obj);
	delete(temp);
	delete(grid_temp);
}

void SolverMPMMulti::InformationReturn()
{
	int csize = sand->particles.size() + water->particles.size();
	particleList.resize(csize);
	for (int i = 0; i < sand->particles.size(); i++)
		particleList[i] = sand->particles[i];
	for (int i = sand->particles.size(); i < csize; i++)
		particleList[i] = water->particles[i - sand->particles.size()];
}

void SolverMPMMulti::MomentaExchange()
{
	double C_e;
	
	memset(cohesion_w, 0, sizeof(double)*grid_s->nodes_length);

	for (int i = 0; i < grid_s->nodes_length; i++)
	{
		if (grid_s->nodes[i].active&&grid_w->nodes[i].active)
		{
			//cout << grid_w->nodes[i].mass << endl;
			C_e = -grid_w->nodes[i].mass * 0.3;
			cohesion_w[i] = 1;
			Matrix2d M = Matrix2d::Zero();
			M(0, 0) = grid_s->nodes[i].mass;
			M(1, 1) = grid_w->nodes[i].mass;
			Matrix2d D = Matrix2d::Zero();
			double Delement = C_e*grid_s->nodes[i].mass*grid_w->nodes[i].mass;
			D(0, 0) = -Delement;
			D(0, 1) = Delement;
			D(1, 0) = Delement;
			D(1, 1) = -Delement;

			Matrix2d V = Matrix2d::Zero();
			V(0, 0) = grid_s->nodes[i].velocity[0];
			V(0, 1) = grid_s->nodes[i].velocity[1];
			V(1, 0) = grid_w->nodes[i].velocity[0];
			V(1, 1) = grid_w->nodes[i].velocity[1];

			Matrix2d G = Matrix2d::Zero();
			G(0, 0) = 0;
			G(0, 1) =-3.0;
			G(1, 0) = 0;
			G(1, 1) = -3.0;

			Matrix2d F = Matrix2d::Zero();
			F(0, 0) = grid_s->nodes[i].force[0];
			F(0, 1) = grid_s->nodes[i].force[1];
			F(1, 0) = grid_w->nodes[i].force[0];
			F(1, 1) = grid_w->nodes[i].force[1];

			Matrix2d A = M + DT*D;
			Matrix2d B = M*V + DT*(M*G - F);
			Matrix2d X = A.inverse()*B;
			grid_s->nodes[i].velocity_new = Vector2d(X(0, 0), X(0, 1));
			grid_w->nodes[i].velocity_new = Vector2d(X(1, 0), X(1, 1));
			//cout << A.inverse() << endl;
		}
		else if(grid_s->nodes[i].active)
		{
			grid_s->nodes[i].velocity_new = grid_s->nodes[i].velocity + DT*(gravity - grid_s->nodes[i].force / grid_s->nodes[i].mass);
		}
		else if (grid_w->nodes[i].active)
		{
			grid_w->nodes[i].velocity_new = grid_w->nodes[i].velocity + DT*(gravity - grid_w->nodes[i].force / grid_w->nodes[i].mass);
		}
	}
	
}

void SolverMPMMulti::Saturation()
{
	//memset(cohesion_s, 0, sizeof(double)*sand->size);
	for (int i = 0; i < sand->size; i++)
	{
		ParticleMPM* p = sand->particles[i];
		p->coh = 0;
		int ox = p->grid_position[0], oy = p->grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					int index = (int)(y*grid_s->size[0] + x);
					p->coh += cohesion_w[index] * w;
				}
			}
		}

	}
}