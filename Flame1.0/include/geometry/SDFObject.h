/*----------- Copyright(c) 2018 YimingXia(491691865@qq.com)-------------------
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once
#include"SDF.h"
#include"../common/Parameters.h"
#include"../simulation/ParticleSample.h"
#include"../physics/Particle.h"

enum SampleType
{
	BOUNDSDF_P,
	BOUNDSDF_O,
	PICTURE_P
};

/********************************************************
	*Class name:       SDFObject
	*Usage:            Repressent rigid body object used in simulation.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
class SDFObject
	:public SDF
{
public:
	SDFObject()
	{
		objectMass = 1.0;
	}
	~SDFObject() {}
	void SDFUpdate(double DT)
	{

	}
	double objectMass;
};

/********************************************************
	*Class name:       SDFSampleBound
	*Usage:            Repressent simulation boundary used in simulation.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
class SDFSampleBound
	:public SDF
{
public:
	SDFSampleBound() {}
	SDFSampleBound(int t, ParticleType particletype, SampleType sampletype, Vector2d v = Vector2d::Zero(), std::string name = " ");
	~SDFSampleBound() {}
	
	SampleType sampleType;          //sample method:possion disc,order,using picture.
	ParticleType particleType;      //particle type:sand,snow,water,elastic
	Vector2d sampleVelocity;        //particles velocity

/********************************************************
	*Function name:    SampleParticle
	*Usage:            Sample particles in the boundary 
	                   which is define by sdf.
	*Input:            None.
	*Output:           Particle coordinate.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	PointList SampleParticle();

/********************************************************
	*Function name:    SDFUpdate
	*Usage:            Nothing to do.
	*Input:            None.
	*Output:           None.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void SDFUpdate(double DT);
	
protected:
	std::string matName;//record the picture name which used to sample particles
};  


/********************************************************
	*Class name:       SDFJet
	*Usage:            Repressent jet used in simulation.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
class SDFJet
	:public SDFSampleBound
{
public:
	SDFJet(int t, ParticleType particletype, SampleType sampletype, Vector2d v, double length, std::string name = " ");
	~SDFJet();
	int sampleTime;          //sample interval time.
	bool sampleCheck;        //a key to mark whether to sample particle.
	int insertTime;          //the time when particles add to scene.
/********************************************************
	*Function name:    SDFUpdate
	*Usage:            Change the jet velocity,sample interval time.
	*Input:            DT.
	*Output:           None.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void SDFUpdate(double DT);
private:
	double jetLength;       //the length of jet,used to calculate sampleTime.
};

class SDFBoundary
	:public SDF
{
public:
	SDFBoundary(int t, SDFType sdftype);
	~SDFBoundary() {}
	double BoundaryDistance(Vector2d p);
	Vector2d BoundaryGradient(double x, double y);
	void SDFUpdate(double DT);
private:
	int contain;
};