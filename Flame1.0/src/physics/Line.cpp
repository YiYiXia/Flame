#include"../../include/physics/Line.h"

Line::Line(SDF* s, SDF* e)
{
	linetype = STRING;
	start = s;
	end = e; 


}
Line::Line(SDF* s, SDF* e, double l, double k, double d) 
{
	linetype = SPRING;
	start = s;
	end = e;

	length_init = l;
	stiffness = k;
	damping = d;
}
Line::~Line()
{
}

void Line::CalculateForce()
{
	start_p = start->center;
	end_p = end->center;
	start_di = (end_p - start_p).normalized();
	end_di = -start_di;
	length = (end_p - start_p).norm();
	//cout <<"length:"<< length << endl;
	force = stiffness*(length - length_init);
}
