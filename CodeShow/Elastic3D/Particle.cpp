#include"Particle.h"
Particle::Particle()
{

}
Particle::Particle(Vector3f pos, Vector3f vel, float mass)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	def_elastic = Matrix3f::Identity();
	svd_u = Matrix3f::Identity();
	svd_v = Matrix3f::Identity();
	svd_s = Matrix3f::Identity();
	polar_r = Matrix3f::Identity();
	polar_s = Matrix3f::Identity();
	B = Matrix3f::Zero();
}
Particle::~Particle()
{

}
//更新粒子位置
void Particle::updatePos()
{
	position += DT*velocity;
}
//形变梯度的更新
void Particle::updateGradient()
{
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient+Matrix3f::Identity();
	def_elastic = velocity_gradient * def_elastic;
}

//计算粒子受力时的关键项
const Matrix3f Particle::energyDerivative()
{
	
	float Je = def_elastic.determinant();
	Matrix3f out;
	//弹性体的本构模型如下：
	Matrix3f first = 2 * mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();//这里的mu是一个参数
	Matrix3f second = lambda*Je*(Je - 1)*Matrix3f::Identity();//这里的lamda是一个参数
	out = first + second;
	return volume * out;

}

//将形变梯度进行SVD分解
void Particle::applyPlasticity()
{
	JacobiSVD<Eigen::MatrixXf>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
}