#pragma once
#define EPSILON 0.01f
#include<Eigen/Dense>
#include<iostream>
#include<vector>
#include"../../include/math/Function.h"

using namespace Eigen;
using namespace std;
//物体、边界、源、汇
enum SDFType
{
	Object,
	ParticleBoundary,
	Boundary,
	Source,
	Sink,
	Rivet,
	Nothing
};



//碰撞类型
enum SDFCollisionType
{
	Sticky,
	Separating,
	Slipping,
	None
};

//运动控制信息，多个运动控制信息组成运动控制脚本
typedef struct SDFMoveInfo
{
	SDFMoveInfo(int st, int et, Vector2d v, double o) :StartTime(st), EndTime(et), veloctiy(v),omega(o){}
	int StartTime;//运动起始时间
	int EndTime;//运动终止时间
	Vector2d veloctiy;//运动速度
	double omega;//角速度
}SDFMoveInfo;


//点到多边形的信息
typedef struct SDFinfo
{
	double distance;
	int num_s;                           //与之最近的线段编号
	int num_e;
} SDFinfo;

double CircleSDF(double x, double y, double c_x, double c_y, double r);
double PlaneSDF(double x, double y, double px, double py, double nx, double ny);
double SegmentSDF(double x, double y, double sx, double sy, double ex, double ey);
double CapsuleSDF(double x, double y, double sx, double sy, double ex, double ey, double r);
double BoxSDF(double x, double y, double cx, double cy, double theta, double sx, double sy);

/********************************************************
	*Class name:       SDF
	*Usage:            Geometry object in scene
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
class SDF
{
public:
	SDF(); 
	~SDF();
	
	std::vector<SDFMoveInfo> moveInfo;                //the move information of SDF,use it to control SDF moving
	std::vector<Vector2d> vertices;                   //vertices coordinate in SDF
	std::vector<Vector2d> verticeVelocity;            //vertice velocity in SDF
	Vector2d barycenterVelocity;                      //barycenter veloctiy
	Vector2d barycenter;                              //barycenter coordinate
	double omega;                                     //omega of the SDF rotate
	bool move;                                        //whether the SDF should move or not
	SDFType type;                                     //the type of SDF
	string sdfName;                                   //the name of SDF
	int appearTime;                                   //the time in which the SDF appear
	double x_min, x_max, y_min, y_max;                //the bounding box of SDF
	Vector3d Color;                                   //the color of SDF

/********************************************************
	*Function name:    Distance
	*Usage:            Calculate the distance between a point to SDF.
	*Input:            point.
	*Output:           distance(inside -,outside +),the nearest side.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	SDFinfo Distance(Vector2d point);

/********************************************************
	*Function name:    Gradient
	*Usage:            Calculate the gradient vector at the point from signed distance field.
	*Input:            point.
	*Output:           the gradient vector(point outside the polygon).
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	Vector2d Gradient(double x, double y);

/********************************************************
	*Function name:    Contains
	*Usage:            Check  whether the point is inside or not.
	*Input:            point.
	*Output:           inside:-1 outside:1
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	int Contains(double x, double y);

/********************************************************
	*Function name:    SDFveloctiy
	*Usage:            Calculate the velocity of the point which is 
	                   nearest from the input point within SDF boundary.
	*Input:            point.
	*Output:           velocity.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	Vector2d SDFveloctiy(double x, double y);

/********************************************************
	*Function name:    Insert
	*Usage:            Insert a vertice into polygon.
	*Input:            vertice coordinate,vertice velocity.
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void Insert(double x, double y, Vector2d v = Vector2d::Zero());

/********************************************************
	*Function name:    AddControl
	*Usage:            Add a move control information into SDF,just like a move script.
	*Input:            SDFMoveInfo include when start,when end,omega and barycenter velocity.
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void AddControl(SDFMoveInfo info);

/********************************************************
	*Function name:    Control
	*Usage:            Use the control information to control SDF move
	*Input:            time(frame number)
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void Control(int time);

/********************************************************
	*Function name:    CalculateBarycenter
	*Usage:            Calculate Barycenter Coordinate
	*Input:            none.
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void CalculateBarycenter();

/********************************************************
	*Function name:    GeometryUpdate
	*Usage:            Use the move information to update SDF geometry information,
	                   such as move,rotate,etc
	*Input:            DT
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void GeometryUpdate(double DT);

/********************************************************
	*Function name:    SDFUpdate(virtual)
	*Usage:            Update different SDF with different method.
	*Input:            DT
	*Output:           none.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	virtual void SDFUpdate(double DT) = 0;
};





