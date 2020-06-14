#pragma once
#include"ParticleCloud.h"
#include"ParticleMPM.h"
#include"ParticleMPMSand.h"
#include"ParticleMPMWater.h"
#include"ParticleMPMElastic.h"
#include"ParticleMPMSnow.h"
#include <fstream>  
#include <sstream>

class ParticleCloudMPM
	:public ParticleCloud
{
public:
	ParticleCloudMPM();
	ParticleCloudMPM(PointList list, ParticleType type, Vector2d velocity, Vector3d PColor = Vector3d(-1, -1, -1));
	~ParticleCloudMPM();
	vector<ParticleMPM*> particles;
	void Merge(ParticleCloudMPM& other);
	void Update();
private:
	void SandParametersIn(SandParameters &p);
	void SnowParametersIn(SnowParameters &p);
	void WaterParametersIn(WaterParameters &p);
	void ElasticParametersIn(ElasticParameters &p);

};