#include"../../include/math/Function.h"

double RandomNumber(double lo, double hi)
{
	return lo + rand() / (double)(RAND_MAX / (hi - lo));
}

double BSpline(double x)
{
	x = fabs(x);
	double w;
	if (x < 1)
		w = x*x*(x / 2 - 1) + 2 / 3.0;
	else if (x < 2)
		w = x*(x*(-x / 6 + 1) - 2) + 4 / 3.0;
	else return 0;
	if (w < 1e-20) return 0;
	return w;
}
double BSplineSlope(double x)
{
	double abs_x = fabs(x);
	if (abs_x < 1)
		return 1.5*x*abs_x - 2 * x;
	else if (x < 2)
		return -x*abs_x / 2 + 2 * x - 2 * x / abs_x;
	else return 0;
}

double LinearKernel(Vector2d d, double cell)
{
	return std::max((1.0 - fabs(d(0) / cell)) * (1.0 - fabs(d(1) / cell)), 0.0);
}

double FractionInside(double phi_left, double phi_right)
{
	if (phi_left < 0 && phi_right < 0)
		return 1;
	if (phi_left < 0 && phi_right >= 0)
		return phi_left / (phi_left - phi_right);
	if (phi_left >= 0 && phi_right < 0)
		return phi_right / (phi_right - phi_left);
	else
		return 0;
}

double Clamp(double a, double s, double e)
{
	if (a < s) return s;
	else if (a > e) return e;
	else return a;

}

double Lerp(Vector2d p, double *a, int width)
{
	int s_x = p[0], s_y = p[1];
	int idx = s_y*width + s_x;
	int idx_r = idx + 1;
	int idx_u = idx + width;
	int idx_ur = idx + width + 1;
	double w_x = p[0] - s_x;
	double w_y = p[1] - s_y;
	double y1, y2;
	y1 = LerpLine(a[idx], a[idx_r], w_x);
	y2 = LerpLine(a[idx_u], a[idx_ur], w_x);
	return  LerpLine(y1, y2, w_y);
}

double LerpLine(double s, double e, double w)
{
	return s*(1 - w) + e*w;
}

Vector2d GradBiLerp(double v00, double v10, double v01, double v11, double fx, double fy)
{
	return Vector2d(fy - 1.0, fx - 1.0) * v00 + Vector2d(1.0 - fy, -fx) * v10 
		+ Vector2d(-fy, 1.0 - fx) * v01 + Vector2d(fy, fx) * v11;
}

Vector2d AffineLerp(Vector2d p, double *a, int width)
{
	int s_x = p[0], s_y = p[1];
	int idx = s_y*width + s_x;
	int idx_r = idx + 1;
	int idx_u = idx + width;
	int idx_ur = idx + width + 1;
	double w_x = p[0] - s_x;
	double w_y = p[1] - s_y;
	return GradBiLerp(a[idx], a[idx_r], a[idx_u], a[idx_ur], w_x, w_y);
}

double SmoothKernel(double r, double h)
{
	return std::max(pow(1.0 - r / (h * h), 3.0), 0.0);
}