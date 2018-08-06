#include"../../include/geometry/SDF.h"

SDF::SDF()
{
	omega = 0;
	mass = 1.0;
	v_center = Vector2d::Zero();
	x_min = 9999999;
	x_max = -9999999;
	y_min= 9999999;
	y_max= -9999999;
	type = Boundary;
	move = false;
}

SDF::SDF(int t , SampleType stype, string name)
{
	omega = 0;
	v_acceleration_new = v_acceleration = v_center_new = v_center = Vector2d::Zero();
	mass = 1.0;
	x_min = 9999999;
	x_max = -9999999;
	y_min = 9999999;
	y_max = -9999999;
	type = Boundary;
	move = false;
	Live_Info.AppearTime = t;
	Sample_Type = stype;
	Mat_name = name;
	sample_time = 0;
}


SDF::SDF(int x, int y)
{
	omega = 0;
	mass = 1.0;
	v_center = Vector2d::Zero();
	grid_sizex = 8*x;
	grid_sizey = 8*y;
	cell_sizex = 0.125;
	cell_sizey = 0.125;
	record = (double*)malloc(grid_sizex*grid_sizey * sizeof(double));
}


SDF::~SDF()
{
}

double CircleSDF(double x, double y, double c_x, double c_y, double r)
{
	double d = sqrt((x - c_x)*(x - c_x) + (y - c_y)*(y - c_y));
	return d - r;
}

double PlaneSDF(double x, double y, double px, double py, double nx, double ny)//平面
{
	return nx*(x - px) + ny*(y - py);
}

double SegmentSDF(double x, double y, double sx, double sy, double ex, double ey)
{
	double vx = x - sx, vy = y - sy;
	double ux = ex - sx, uy = ey - sy;
	double p = fmax(fmin((ux*vx + uy*vy) / (ux*ux + uy*uy), 1), 0);
	double px = sx + p*ux, py = sy + p*uy;
	return sqrt((x - px)*(x - px) + (y - py)*(y - py));
}

double CapsuleSDF(double x, double y, double sx, double sy, double ex, double ey, double r)
{
	return SegmentSDF(x, y, sx, sy, ex, ey) - r;
}



double BoxSDF(double x, double y, double cx, double cy, double theta, double sx, double sy) {
	double costheta = cosf(theta), sintheta = sinf(theta);
	double dx = fabs((x - cx) * costheta + (y - cy) * sintheta) - sx;
	double dy = fabs((y - cy) * costheta - (x - cx) * sintheta) - sy;
	double ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
	return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
}

void SDF::Insert(double x, double y, Vector2d v)
{
	Vector2d p = Vector2d(x, y);
	vertices.push_back(p);
	velocity.push_back(v);
	x_min = (x > x_min) ? x_min : x;
	x_max = (x < x_max) ? x_max : x;
	y_min = (y > y_min) ? y_min : y;
	y_max = (y < y_max) ? y_max : y;

}

SDFinfo SDF::Distance(Vector2d point)
{
	SDFinfo info;
	double d = 10000;
	int n = vertices.size() - 1, m = 0;
	for (int i = vertices.size() - 1, j = 0; j < vertices.size(); i = j, j++)
	{
		double l = SegmentSDF(point[0], point[1], vertices[i][0], vertices[i][1], vertices[j][0], vertices[j][1]);
		if (d > l)
		{
			d = l;
			m = i;
			n = j;
		}
	}
	int sign = Contains(point[0], point[1]);
	info.distance = sign*d;
	info.num_s = m;
	info.num_e = n;
	return info;
}




Vector2d SDF::Gradient(double x, double y)
{
	double nx, ny;
	nx = (Distance(Vector2d(x + EPSILON, y)).distance - Distance(Vector2d(x - EPSILON, y)).distance)*0.5 / EPSILON;
	ny = (Distance(Vector2d(x, y + EPSILON)).distance - Distance(Vector2d(x, y - EPSILON)).distance)*0.5 / EPSILON;
	double l = sqrt(nx*nx + ny*ny);
	nx /= l;
	ny /= l;
	return Vector2d(nx, ny);
}

int SDF::Contains(double x, double y)
{
	int result = 1;
	int len = vertices.size();
	for (int i = 0, j = len - 1; i<len; j = i++)
	{
		Vector2d &vi = vertices[i], &vj = vertices[j];
		if ((vi[1] > y) != (vj[1] > y) && (x < (vj[0] - vi[0]) * (y - vi[1]) / (vj[1] - vi[1]) + vi[0])) //前半部分：点被夹在一条线段的范围内 后半部分：点与线段上的临近点比较
		{
			result *= -1;
		}
	}
	return result;
}

Vector2d SDF::SDFveloctiy(double x, double y)
{
	Vector2d p(x, y);
	SDFinfo re = Distance(Vector2d(x, y));
	Vector2d start = vertices[re.num_s];
	Vector2d end = vertices[re.num_e];
	Vector2d line = (end - start).normalized();
	Vector2d v_start = velocity[re.num_s];
	Vector2d v_end = velocity[re.num_e];
	Vector2d v_tangent = v_start.dot(line)*line;
	Vector2d v_start_normal = v_start - v_tangent;
	Vector2d v_end_normal = v_end - v_tangent;
	double ls = (p - start).norm();
	double le = (p - end).norm();
	Vector2d vp_normal = (le*v_start_normal + ls*v_end_normal) / (le + ls);
	return vp_normal + v_tangent;

}

void SDF::Initialize()
{

	double area = 0,
		s,
		len = vertices.size(),
		x = 0,
		y = 0,
		a,
		b,
		d, cx, cy, A, B, f1, f2, f3, f4;
	//用正负梯形来计算多边形的质心
	for (int i = 0, j = len - 1; i<len; j = i++)
	{
		Vector2d &vi = vertices[i], &vj = vertices[j];
		s = (vj[1] + vi[1]) * (vi[0] - vj[0]) / 2;
		if (fabs(vi[0] - vj[0]) <= 1e-6) continue;
		area += s;
		a = vj[1];
		b = vi[1];
		d = vi[0] - vj[0];
		cx = d*(a / 3 + 2 * b / 3) / (a + b) + vj[0];
		cy = (a*a + a*b + b*b) / 3 / (a + b);
		x += cx*s;
		y += cy*s;
	}
	center[0] = x / area;
	center[1] = y / area;
}

void SDF::Update(double DT)
{
	std::vector<Vector2d> relate;
	for (int i = 0; i < vertices.size(); i++)
		relate.push_back(vertices[i] - center);//记录顶点相对中心的半径
	//计算顶点速度
	for (int i = 0; i < vertices.size(); i++)
	{
		velocity[i][0] = -omega*(vertices[i][1] - center[1]) + v_center[0];
		velocity[i][1] = omega*(vertices[i][0] - center[0]) + v_center[1];
	}
	
	//更新中心位置
	center = v_center*DT + center;
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = center + Rotate(relate[i], omega*DT);
		x_min = (vertices[i][0] > x_min) ? x_min : vertices[i][0];
		x_max = (vertices[i][0] < x_max) ? x_max : vertices[i][0];
		y_min = (vertices[i][1] > y_min) ? y_min : vertices[i][1];
		y_max = (vertices[i][1] < y_max) ? y_max : vertices[i][1];
	}
		

	
}


Vector2d SDF::Rotate(Vector2d p, double theata)
{
	Matrix2d r;
	r(0, 0) = cos(theata);
	r(0, 1) = -sin(theata);
	r(1, 0) = sin(theata);
	r(1, 1) = cos(theata);
	return r*p;
}

void SDF::GirdInitial()
{
	for (int i = 0; i < grid_sizex; i++)
		for (int j = 0; j < grid_sizey; j++)
			record[j*grid_sizex + i] = Distance(Vector2d(i*cell_sizex, j*cell_sizey)).distance;
}

double SDF::GridDistance(double x, double y)
{
	double s1 = x / cell_sizex;
	double s2 = y / cell_sizey;
	int i = s1;
	int j = s2;
	if (i<0 || i>=(grid_sizex - 1) || j<0 || j>=(grid_sizey - 1)) return 100.0;
	int p1 = j*grid_sizex + i, p2 = j*grid_sizex + i + 1, p3 = (j + 1)*grid_sizex + i, p4 = (j + 1)*grid_sizex + i + 1;
	//双线性插值
	double r1, r2;
	r1 = record[p2] * (s1 - i) + record[p1] * (1 + i - s1);
	r2 = record[p4] * (s1 - i) + record[p3] * (1 + i - s1);
	return r2*(s2 - j) + r1*(1 + j - s2);
}

void SDF::AddControl(SDFMoveInfo info){
	move = true;
	move_info.push_back(info);
}

void SDF::Control(int time)
{
	for (int i = 0; i < move_info.size(); i++)
	{
		if (move_info[i].StartTime < time&&move_info[i].EndTime>time)
		{
			v_center = move_info[i].veloctiy;
			omega = move_info[i].omega;
		}
	}
}