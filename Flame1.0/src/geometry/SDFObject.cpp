#include"../../include/geometry/SDFObject.h"

SDFSampleBound::SDFSampleBound(int t, ParticleType particletype, SampleType sampletype, Vector2d v, std::string name)
{
	sampleVelocity = v;
	appearTime = t;
	type = ParticleBoundary;
	sampleType = sampletype;
	matName = name;
	particleType = particletype;
}

PointList SDFSampleBound::SampleParticle()
{
	PointList list;
	do {
		if (sampleType == PICTURE_P)
		{
			cv::Mat record = cv::imread(matName);
			list = PossionDisc(r, record, this, iteratime);
		}
		if (sampleType == BOUNDSDF_P)
		{
			list = PossionDisc(r, this, iteratime);
		}
		if (sampleType == BOUNDSDF_O)
		{
			list = OrderSample(this);
		}
	} while (list.size() < 20);
	return list;
}

void SDFSampleBound::SDFUpdate(double DT)
{
}

SDFJet::SDFJet(int t, ParticleType particletype, SampleType sampletype, Vector2d v, double length, string name)
{
	appearTime = t;
	insertTime = t;
	type = Source;
	sampleType = sampletype;
	//cout <<"sample num:" <<sampleType << endl;
	matName = name;
	sampleVelocity = v;  
	jetLength = length;
	sampleTime = jetLength / (sampleVelocity.norm()*DT);
	sampleCheck = false;
	particleType = particletype;
}

SDFJet::~SDFJet()
{
}

void SDFJet::SDFUpdate(double DT)
{
	GeometryUpdate(DT);
	sampleVelocity = Rotate(sampleVelocity,omega*DT);
	if (sampleCheck == true)
	{
		sampleTime = jetLength / (sampleVelocity.norm()*DT);
		sampleCheck = false;
	}
}

SDFBoundary::SDFBoundary(int t, SDFType sdftype)
{
	appearTime = t;
	type = sdftype;
	if (type == Boundary|| type == Sink) contain = 1;
	else if (type == Object || type == Rivet) contain = -1;
}

double SDFBoundary::BoundaryDistance(Vector2d p)
{
	return contain*Distance(p).distance;
}

Vector2d SDFBoundary::BoundaryGradient(double x, double y)
{
	return contain*Gradient(x, y);
}

void SDFBoundary::SDFUpdate(double DT)
{
	GeometryUpdate(DT);
}