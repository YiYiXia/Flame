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
#include<map>

#include"../physics/Particle.h"
#include"../common/Parameters.h"
#include"../physics/Line.h"
#include"../render/Camera2D.h"
#include"../geometry/SDF.h"
#include"../geometry/SDFObject.h"

using namespace std;

//todo:color is not the SDF color,to be more useful,it should be modify.
typedef struct SDFInfo 
{
	SDF* sdf;
	SDFType stype;
	Vector3d color;
	SDFInfo()
	{
		stype = Nothing;
		color = Vector3d(-1, -1, -1);
	}
}SDFInfo;                                        //the information of SDF used in the scene


/********************************************************
	*Class name:	   Scene
	*Usage:			   Used to save and manage objects in scene,Object,Jet,Boundary
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
class Scene
{
public:
	Scene();
	~Scene();
	vector<SDFInfo> SDFlist;                     //list of sdfinfo in scene
	vector<SDFObject*> SDFObjectList;            //list of Object in scene
	vector<SDFSampleBound*> SDFSampleBoundList;  //list of SampleBound in scene
	vector<SDFJet*> SDFJetList;                  //list of Jet in scene
	vector<SDFBoundary*> SDFBoundaryList;        //list of Boundary in scene
	vector<Particle*> pointlist;                 //list of particles in scene
	vector<Line*> linelist;                      //list of line object in scene
	Camera camera;                               //camera in scene



/********************************************************
	*Function name:    AddObject
	*Usage:            Add Objects into scene.
	*Input:            Overload:          +4
					   Objetc:            (1)Object
					   Particle Boundary: (2)Particle boundary,Particle Color
					   Jet:               (3)Jet boundary,Particle Color
					   Boundary:          (4)Boundary
	*Output:           None.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void AddObject(
		SDFObject* polygon);//Object

	void AddObject(
		SDFSampleBound* polygon,
		Vector3d ColorFill = Vector3d(-1, -1, -1));//SampleBound

	void AddObject(
		SDFJet* polygon,
		Vector3d ColorFill = Vector3d(-1, -1, -1));//Jet

	void AddObject(
		SDFBoundary* polygon);//Boundary

/********************************************************
	*Function name:    FindObject,FindBoundary,FindJet
	*Usage:            Find the sdf in scene with its name.
	*Input:            Name.
	*Output:           A pointer point to the sdf.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	SDFBoundary* FindBoundary(string name);
	SDFJet* FindJet(string name);
	SDFObject* FindObject(string name);


/********************************************************
	*Function name:    SceneUpdate
	*Usage:            The sdf in scene should update their information,this function 
	                   check whether to change omega,change omega or not.
	*Input:            None.
	*Output:           None.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void SceneUpdate();                          //Update objects in scene


};