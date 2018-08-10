#include"../../include/geometry/SDF.h"

SDF::SDF()
{
	omega = 0;
	barycenterVelocity = Vector2d::Zero();
	x_min = 9999999;
	x_max = -9999999;
	y_min= 9999999;
	y_max= -9999999;
	move = false;
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
	verticeVelocity.push_back(v);
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
	Vector2d v_start = verticeVelocity[re.num_s];
	Vector2d v_end = verticeVelocity[re.num_e];
	Vector2d v_tangent = v_start.dot(line)*line;
	Vector2d v_start_normal = v_start - v_tangent;
	Vector2d v_end_normal = v_end - v_tangent;
	double ls = (p - start).norm();
	double le = (p - end).norm();
	Vector2d vp_normal = (le*v_start_normal + ls*v_end_normal) / (le + ls);
	return vp_normal + v_tangent;

}

void SDF::CalculateBarycenter()
{
	double area = 0,
		s,
		len = vertices.size(),
		x = 0,
		y = 0,
		a,
		b,
		d, cx, cy;
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
	barycenter[0] = x / area;
	barycenter[1] = y / area;
}

void SDF::GeometryUpdate(double DT)
{
	std::vector<Vector2d> relate;
	for (int i = 0; i < vertices.size(); i++)
		relate.push_back(vertices[i] - barycenter);//记录顶点相对中心的半径
	//计算顶点速度
	for (int i = 0; i < vertices.size(); i++)
	{
		verticeVelocity[i][0] = -omega*(vertices[i][1] - barycenter[1]) + barycenterVelocity[0];
		verticeVelocity[i][1] = omega*(vertices[i][0] - barycenter[0]) + barycenterVelocity[1];
	}
	
	//更新中心位置
	barycenter = barycenterVelocity*DT + barycenter;
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = barycenter + Rotate(relate[i], omega*DT);
		x_min = (vertices[i][0] > x_min) ? x_min : vertices[i][0];
		x_max = (vertices[i][0] < x_max) ? x_max : vertices[i][0];
		y_min = (vertices[i][1] > y_min) ? y_min : vertices[i][1];
		y_max = (vertices[i][1] < y_max) ? y_max : vertices[i][1];
	}
}

void SDF::AddControl(SDFMoveInfo info){
	move = true;
	moveInfo.push_back(info);
}

void SDF::Control(int time)
{
	for (int i = 0; i < moveInfo.size(); i++)
	{
		if (moveInfo[i].StartTime < time&&moveInfo[i].EndTime>time)
		{
			barycenterVelocity = moveInfo[i].veloctiy;
			omega = moveInfo[i].omega;
		}
	}
}

