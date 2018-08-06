#include"../../include/physics/ParticleMPMElastic.h"

ParticleMPMElastic::ParticleMPMElastic()
{
}
ParticleMPMElastic::~ParticleMPMElastic()
{
}

ParticleMPMElastic::ParticleMPMElastic(Vector2d pos, Vector2d vel, double mass, int num, double lamda, double mu)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	index = num;
	Lambda = lamda;
	Mu = mu;
	color = Vector3d(0.4, 0.8, 0.2);
	p_type = ELASTIC;
}

ParticleMPMElastic::ParticleMPMElastic(Vector2d pos, Vector2d vel, int num, ElasticParameters p, Vector3d PColor)
{
	position = pos;
	velocity = vel;
	index = num;
	this->mass = p.mass;
	Lambda = p.Lambda;
	Mu = p.Mu;
	if (PColor[0] < 0) color = Vector3d(0.0, 0.67, 0.67);
	else color = PColor;
	p_type = ELASTIC;
}

Matrix2d ParticleMPMElastic::EnergyDerivative()
{
	double Je = def_elastic.determinant();
	Matrix2d out;
	Matrix2d first = 2 * Mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	Matrix2d second = Lambda*Je*(Je - 1)*Matrix2d::Identity();
	out = first + second;
	return volume * out;
}

void ParticleMPMElastic::MPMUpdate()
{
	UpdatePos();
	UpdateGradient();
	ApplyPlasticity();
}
void ParticleMPMElastic::DeltaForce()
{
	Matrix2d del_elastic = DT*delta_v*def_elastic;
	double y = (polar_r(0, 0) * del_elastic(0, 1) + polar_r(1, 0) * del_elastic(1, 1)) -
		(polar_r(0, 1) * del_elastic(0, 0) + polar_r(1, 1) * del_elastic(0, 1));
	double x = y / (polar_s(0, 0) + polar_s(1, 1));
	Matrix2d del_rotate;
	del_rotate(0, 0) = -polar_r(0, 1) * x;
	del_rotate(0, 1) = polar_r(0, 0) * x;
	del_rotate(1, 0) = -polar_r(1, 1) * x;
	del_rotate(1, 1) = polar_r(1, 0) * x;


	//JF^-T
	Matrix2d cofactor;
	cofactor(0, 0) = def_elastic(1, 1);
	cofactor(0, 1) = -def_elastic(1, 0);
	cofactor(1, 0) = -def_elastic(0, 1);
	cofactor(1, 1) = def_elastic(0, 0);

	Matrix2d del_cofactor;
	del_cofactor(0, 0) = del_elastic(1, 1);
	del_cofactor(0, 1) = -del_elastic(1, 0);
	del_cofactor(1, 0) = -del_elastic(0, 1);
	del_cofactor(1, 1) = del_elastic(0, 0);
	//cout << u[0] << endl;
	Matrix2d Ap = del_elastic - del_rotate;
	Ap *= 2 * Mu;
	Matrix2d temp = cofactor*del_elastic.transpose();//矩阵对应元素相乘求和

	cofactor *= temp.trace();
	del_cofactor *= (def_elastic.determinant() - 1);
	cofactor += del_cofactor;
	cofactor *= Lambda;
	Ap += cofactor;

	delta_f = volume*(Ap*def_elastic.transpose());
}

void ParticleMPMElastic::UpdatePos()
{
	position += DT*velocity;
}
void ParticleMPMElastic::UpdateGradient()
{
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient + Matrix2d::Identity();
	def_elastic = velocity_gradient * def_elastic;
}
void ParticleMPMElastic::ApplyPlasticity()
{
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
}