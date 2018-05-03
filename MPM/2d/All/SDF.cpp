#include"SDF.h"


SDF::SDF()
{
	omega = 0;
	v_center = Vector2d::Zero();
}


SDF::~SDF()
{
		
}

double SDF::SegmentSDF(double x, double y, double sx, double sy, double ex, double ey)
{
	double vx = x - sx, vy = y - sy;
	double ux = ex - sx, uy = ey - sy;
	double p = fmax(fmin((ux*vx + uy*vy) / (ux*ux + uy*uy), 1), 0);
	double px = sx + p*ux, py = sy + p*uy;
	return sqrt((x - px)*(x - px) + (y - py)*(y - py));
}


void SDF::Insert(double x, double y, Vector2d v)
{
	Vector2d p = Vector2d(x, y);
	vertices.push_back(p);
	velocity.push_back(v);
}

SDFinfo SDF::Distance(Vector2d point)
{
	SDFinfo info;
	double d = 10000;
	int n, m;
	for (int i = vertices.size() - 1, j = 0; j < vertices.size() ; i = j, j++)
	{
		double l = SegmentSDF(point[0], point[1], vertices[i][0], vertices[i][1], vertices[j][0], vertices[j][1]);
		if (d > l)
		{
			d = l;
			m = i;
			n = j;
		}
		//d = (d > l) ? l : d;
	}
	int sign = Contains(point[0], point[1]);
	info.distance= sign*d;
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
	//int sign = Contains(x, y);
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
		//if ((vi[1] > y) != (vj[1] > y) && (x == (vj[0] - vi[0]) * (y - vi[1]) / (vj[1] - vi[1]) + vi[0])) //点在边界上，默认在点外
		//{
		//	return 0;
		//}
		//if ((vi[1] >= y) != (vj[1] > y) && (vi[1] == vj[1]) && (fabs(x - vj[0]) <= fabs(vi[0] - vj[0])))
		//{
		//	return 0;
		//}
	}
	return result;
}

Vector2d SDF::SDFveloctiy(double x, double y)
{
	Vector2d p(x, y);
	SDFinfo re = Distance(Vector2d(x, y));
	//int sign = Contains(x, y);
	//Vector2d normal = sign*Gradient(x, y);
	//Vector2d q = p - re.distance*normal;

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
		//cout << fabs(vi[0] - vj[0]) << endl;
		if (fabs(vi[0] - vj[0]) <= 1e-6)
		{
			continue;
		}
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
	//std::cout << center[0] <<"  "<< center[1] << std::endl;
}

void SDF::Update(double DT)
{
	std::vector<Vector2d> relate;
	for (int i = 0; i < vertices.size(); i++)
	{
		relate.push_back(vertices[i] - center);//记录顶点相对中心的半径

	}
	//计算顶点速度
	for (int i = 0; i < vertices.size(); i++)
	{
		velocity[i][0] = -omega*(vertices[i][1] - center[1]) + v_center[0];
		velocity[i][1] = omega*(vertices[i][0] - center[0]) + v_center[1];
	}
	//更新中心位置
	center = v_center*DT + center;
	//
	for(int i=0;i<vertices.size();i++)
	{

		vertices[i] = center + Rotate(relate[i], omega*DT);
	}
}

void SDF::Draw()
{
	glColor3f(1, .3, 1);
	glPointSize(1.5f);
	glBegin(GL_POINTS);
	for (int i = 0, l = vertices.size(); i<l; i++)
		glVertex2f(vertices[i][0], vertices[i][1]);
	glEnd();
	glLineWidth(2);
	glBegin(GL_LINES);
	for (int i = 0, j = vertices.size() - 1, l = vertices.size(); i < l; j = i++)
	{
		glVertex2f(vertices[j][0], vertices[j][1]);
		glVertex2f(vertices[i][0], vertices[i][1]);
	}

	glEnd();
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