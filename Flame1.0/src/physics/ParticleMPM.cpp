#include"../../include/physics/ParticleMPM.h"

ParticleMPM::ParticleMPM()
{
	def_elastic = Matrix2d::Identity();
	def_plastic = Matrix2d::Identity();
	svd_u = Matrix2d::Identity();
	svd_v = Matrix2d::Identity();
	svd_s = Matrix2d::Identity();
	polar_r = Matrix2d::Identity();
	polar_s = Matrix2d::Identity();
	B = Matrix2d::Zero();
	volume_check = false;
	coh = 0;
}
ParticleMPM::~ParticleMPM()
{
}