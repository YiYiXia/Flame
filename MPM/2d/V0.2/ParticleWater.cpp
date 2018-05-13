#include"ParticleWater.h"

ParticleWater::ParticleWater()
{
}


ParticleWater::~ParticleWater()
{
}

ParticleWater::ParticleWater(Vector2d pos, Vector2d vel, double mass, int num, double gama, double hardness)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	index = num;
	Gama = gama;
	Hardness = hardness;
	J = 1;
	color = Vector3d(0, 0.3, 0.7);
}


void ParticleWater::updatePos()
{
	position += DT*velocity;
}
void ParticleWater::updateGradient()
{
	double trace;
	trace = velocity_gradient.trace();
	trace *= DT;
	trace = trace + 1;
	J = trace * J;
}

Matrix2d ParticleWater::energyDerivative()
{
	double Je = def_elastic.determinant();
	Matrix2d out;

	double P = Hardness*(1 / pow(J, Gama) - 1);
	Matrix2d temp = Matrix2d::Identity();
	//Matrix2d first = 2 * mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	//Matrix2d second = lambda*Je*(Je - 1)*Matrix2d::Identity();
	//Add in the primary contour term
	out = -P*temp;
	//Add hardening and volume
	/*if (Je < 0.1)
	{
	cout << Je << endl;
	}*/

	return volume * out;
}

void ParticleWater::Update()
{
	updatePos();
	updateGradient();
}