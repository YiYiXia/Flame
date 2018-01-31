#include"Particle.h"
Particle::Particle()
{

}
Particle::Particle(Vector2f pos, Vector2f vel, float mass, int num)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	def_elastic = Matrix2f::Identity();
	def_plastic = Matrix2f::Identity();
	svd_u = Matrix2f::Identity();
	svd_v = Matrix2f::Identity();
	svd_s = Matrix2f::Identity();
	polar_r = Matrix2f::Identity();
	polar_s = Matrix2f::Identity();
	index = num;
	B = Matrix2f::Zero();
}
Particle::~Particle()
{

}

void Particle::updatePos()
{
	position += DT*velocity;
	/*if (ji >= 52)
	{
		cout << ji << "  " << velocity[0] << "  " << velocity[1] << endl << endl;
	}*/
	//cout << velocity[0] << "  " << velocity[1] << endl;
}

void Particle::updateGradient()
{
	/*if (ji >= 52)
	{
		cout << ji << "  " << index << "  " << velocity_gradient(0, 0) << "  " << velocity_gradient(0, 1) << "  " << velocity_gradient(1, 0) << "  " << velocity_gradient(1, 1) << endl ;
	}*/
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient+Matrix2f::Identity();

	def_elastic = velocity_gradient * def_elastic;
}

const Matrix2f Particle::energyDerivative()
{
	float harden = exp(HARDENING*(1 - def_plastic.determinant()));
	/*if (ji >= 52)
	{
		cout << ji << "  " << index << "  " << def_plastic(0, 0) << "  " << def_plastic(0, 1) << "  " << velocity_gradient(1, 0) << "  " << velocity_gradient(1, 1) << endl;
	}*/
	float Je = def_elastic.determinant();
	Matrix2f out;
	Matrix2f first = 2 * mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	Matrix2f second = lambda*Je*(Je - 1)*Matrix2f::Identity();
	//Add in the primary contour term
	out = first + second;
	//Add hardening and volume
	/*if (Je < 0.1)
	{
		cout << Je << endl;
	}*/
	/*if (ji >= 52)
	{
		cout << ji << "  " << index << "  " << def_plastic.determinant() << "  " << harden << endl;
	}*/
	return volume *harden* out;

}

void Particle::applyPlasticity()
{
	Matrix2f def_all = def_elastic * def_plastic;
	JacobiSVD<Eigen::MatrixXf>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();
	//if (ji >= 52)
	//{
	//	cout << ji << "  "<<index << "  " << def_elastic(0, 0) << "  " << def_elastic(0, 1) << "  " << def_elastic(1, 0) << "  " << def_elastic(1, 1) << endl;
	//}
	for (int i = 0; i < 2; i++)
	{
		if (svd_s(i,i) < CRIT_COMPRESS)
			svd_s(i, i) = CRIT_COMPRESS;
		else if (svd_s(i, i) > CRIT_STRETCH)
			svd_s(i, i) = CRIT_STRETCH;
	}
	Matrix2f temp = svd_s;
	//if (ji >= 52)
	//{
	//	cout << ji << "  " << svd_s(0, 0) << "  " << svd_s(1, 1) << endl << endl;
	//}
	
	temp(0, 0) = 1 / temp(0, 0);
	temp(1, 1) = 1 / temp(1, 1);
	def_elastic = svd_u*svd_s*svd_v.transpose();
	def_plastic = svd_v*temp*svd_u.transpose()*def_all;
#ifdef debs
	/*if (ji >= 52)
	{
		cout << ji << "  " << index << "  " << def_plastic(0, 0) << "  " << def_plastic(0, 1) << "  " << def_plastic(1, 0) << "  " << def_plastic(1, 1) << endl;
	}*/
#endif // deb

	

	/*cout << svd_s << endl;*/
}