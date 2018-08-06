#pragma once
#include"../geometry/SDF.h"
enum LineType
{
	STRING,
	SPRING
};
class Line
{
public:
	Line(SDF* s, SDF* e);
	Line(SDF* s, SDF* e, double l, double k, double d);
	~Line();
	void CalculateForce();
	double length, length_init;
	double stiffness;
	double damping;
	LineType linetype;
	SDF* start;
	SDF* end;
	Vector2d start_p;
	Vector2d end_p;
	Vector2d start_di, end_di;
	double force;
};