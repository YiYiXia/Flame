#pragma once
#define EPSILON 0.01f
#include<Eigen/Dense>
#include<iostream>
#include<vector>

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
	Rivet
};

//采样方式
enum SampleType
{
	BOUNDSDF_P,
	PICTURE_P,
	BOUNDSDF_O
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

//存活信息，什么时候出现、消失
typedef struct SDFLiveInfo 
{
	int AppearTime;
}SDFLiveInfo;

//点到多边形的信息
typedef struct SDFinfo
{
	double distance;
	int num_s;//与之最近的线段编号
	int num_e;
} SDFinfo;

double CircleSDF(double x, double y, double c_x, double c_y, double r);
double PlaneSDF(double x, double y, double px, double py, double nx, double ny);//平面
double SegmentSDF(double x, double y, double sx, double sy, double ex, double ey);
double CapsuleSDF(double x, double y, double sx, double sy, double ex, double ey, double r);
double BoxSDF(double x, double y, double cx, double cy, double theta, double sx, double sy);

class SDF
{
public:
	std::vector<SDFMoveInfo> move_info;
	std::vector<Vector2d> vertices;//存放多边形顶点
	std::vector<Vector2d> velocity;//存放顶点的速度
	Vector2d v_center, v_center_new;
	Vector2d v_acceleration, v_acceleration_new;
	SDF(); 
	SDF(int t ,SampleType stype=BOUNDSDF_P, string name = " ");//设置出现时间
	SDF(int x, int y);//为了搞光线追踪用的，后面需要调整
	~SDF();

	Vector2d center;//纯几何图形的质心位置 
	double mass;
	double omega;
	bool move;
	SDFType type;//SDF的类型
	SDFLiveInfo Live_Info;
	SampleType Sample_Type;
	string Mat_name;


	//辅助计算SDF
	int grid_sizex,grid_sizey;
	double cell_sizex, cell_sizey;
	double* record;
	double x_min, x_max, y_min, y_max;//多边形的外接边框
	Vector3d Color;

	int sample_time;
	bool sample_check;
	//SDF与其他
	SDFinfo Distance(Vector2d point);//计算点到多边形的距离，内部距离为负，外部距离为正
	Vector2d Gradient(double x, double y);//计算该点处的梯度，一律指向多边形外
	int Contains(double x, double y);//查看某点是否在多边形内部,内部为-1，外部为1
	Vector2d SDFveloctiy(double x, double y);//获取临近点

	//SDF自身特性
	void Insert(double x, double y, Vector2d v = Vector2d::Zero());//新增一个顶点
	void AddControl(SDFMoveInfo info);//添加SDF的运动信息
	void Control(int time);//根据控制信息，改变运动信息
	void Update(double DT);//更新多边形顶点位置、速度 
	void Initialize();
	void GirdInitial();
	double GridDistance(double x, double y);
	Vector2d Rotate(Vector2d p, double theata);
private:
	
	

};

