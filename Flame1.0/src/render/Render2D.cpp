#include"../../include/render/Render2D.h"

Render2D::Render2D(int h, int w)
	:height(h), width(w)
{
	canvas = cv::Mat::zeros(height, width, CV_8UC3);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			canvas.at<cv::Vec3b>(j, i)(0) = 255;
			canvas.at<cv::Vec3b>(j, i)(1) = 255;
			canvas.at<cv::Vec3b>(j, i)(2) = 255;
		}
	}	
}

Render2D::~Render2D()
{

}
void Render2D::CanvasClear()
{
	canvas = cv::Mat::zeros(height, width, CV_8UC3);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			canvas.at<cv::Vec3b>(j, i)(0) = 255;
			canvas.at<cv::Vec3b>(j, i)(1) = 255;
			canvas.at<cv::Vec3b>(j, i)(2) = 255;

		}
	}
}

void Render2D::CanvasDraw(Scene* sc)
{

	double cell_x = sc->camera.window_cell_x;
	double cell_y = sc->camera.window_cell_y;
	for (int i = 0; i < sc->pointlist.size(); i++)
	{  
		Vector2i relate_position = sc->camera.Project(sc->pointlist[i]->position);
		int cx = relate_position[0];
		int cy = height - relate_position[1]; 
		if (cx<0 || cx>=width || cy<0 || cy>=height) continue;
		int R = sc->pointlist[i]->color[0] * 255;
		int G = sc->pointlist[i]->color[1] * 255;
		int B = sc->pointlist[i]->color[2] * 255;
		if (sc->pointlist[i]->p_type == WATER)
		{
			if (canvas.at<cv::Vec3b>(cy, cx)(0) == 255&& canvas.at<cv::Vec3b>(cy, cx)(1) == 255&& canvas.at<cv::Vec3b>(cy, cx)(2) == 255)
			{
				cv::circle(canvas, cv::Point(cx, cy), 1, cv::Scalar(B, G, R), -1);
			}
			else
			{
				
				//cout << cy<<" "<<cx<<" "<< canvas.at<cv::Vec3b>(cy, cx)(0) << " " << canvas.at<cv::Vec3b>(cy, cx)(1) << " " << canvas.at<cv::Vec3b>(cy, cx)(1) << endl;
				R = 0.8*canvas.at<cv::Vec3b>(cy, cx)(2) + 0.2*R;
				G = 0.8*canvas.at<cv::Vec3b>(cy, cx)(1) + 0.2*G;
				B = 0.8*canvas.at<cv::Vec3b>(cy, cx)(0) + 0.2*B;
				cv::circle(canvas, cv::Point(cx, cy), 1, cv::Scalar(B, G, R), -1);
			}
		}
		else
		{
			cv::circle(canvas, cv::Point(cx, cy), 1, cv::Scalar(B, G, R), -1);
		}
		
		
	}
	for (int i = 0; i < sc->linelist.size(); i++)
	{
		Vector2i start_relate_position = sc->camera.Project(sc->linelist[i]->start_p);
		int sx = start_relate_position[0];
		int sy = height - start_relate_position[1];
		Vector2i end_relate_position = sc->camera.Project(sc->linelist[i]->end_p);
		int ex = end_relate_position[0];
		int ey = height - end_relate_position[1];
		//if (sx<0 || sx >= width || sy<0 || sy >= height) continue;
		cv::line(canvas, cv::Point(sx, sy), cv::Point(ex, ey), cv::Scalar(205, 252, 107), 3.0*sc->camera.Scale());
	}
	for (int i = 0; i < sc->SDFlist.size(); i++)
	{
		for (int j = 0, k = sc->SDFlist[i].sdf->vertices.size() - 1, l = sc->SDFlist[i].sdf->vertices.size(); j < l; k = j++)
		{
			Vector2i start_relate_position = sc->camera.Project(sc->SDFlist[i].sdf->vertices[k]);
			int sx = start_relate_position[0];
			int sy = height - start_relate_position[1];
			Vector2i end_relate_position = sc->camera.Project(sc->SDFlist[i].sdf->vertices[j]);
			int ex = end_relate_position[0];
			int ey = height - end_relate_position[1];

			if (sc->SDFlist[i].sdf->type == Object){
				cv::line(canvas, cv::Point(sx, sy), cv::Point(ex, ey), cv::Scalar(253, 185, 255), 6.0 * sc->camera.Scale());
			}
			if (sc->SDFlist[i].sdf->type == Boundary){
				cv::line(canvas, cv::Point(sx, sy), cv::Point(ex, ey), cv::Scalar(253, 185, 134), 2.0 * sc->camera.Scale());
			}
			if (sc->SDFlist[i].sdf->type == Source) {
				cv::line(canvas, cv::Point(sx, sy), cv::Point(ex, ey), cv::Scalar(100, 185, 255), 2.0 * sc->camera.Scale());
			}
			if (sc->SDFlist[i].sdf->type == Rivet) {
				cv::line(canvas, cv::Point(sx, sy), cv::Point(ex, ey), cv::Scalar(255, 164, 225), 6.0 * sc->camera.Scale());
			}
		}
	}
}

void Render2D::CanvasDraw(vector<Vector2d> pointlist)
{
	double cell_x = VIEW_WIDTH / width;
	double cell_y = VIEW_HEIGHT / height;
	for (int i = 0; i < pointlist.size(); i++)
	{
		int cx = pointlist[i][0] / cell_x;
		int cy = height - pointlist[i][1] / cell_y;
		cv::circle(canvas, cv::Point(cx,cy),3 , cv::Scalar(255, 255, 255), -1);
	}
	cv::GaussianBlur(canvas, canvas, cv::Size(17, 17), 3, 3);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (canvas.at<cv::Vec3b>(j, i)(0) > 130)
			{
				canvas.at<cv::Vec3b>(j, i)(0) = 255;
				canvas.at<cv::Vec3b>(j, i)(1) = 255;
				canvas.at<cv::Vec3b>(j, i)(2) = 255;
			}
			else
			{
				canvas.at<cv::Vec3b>(j, i)(0) = 0;
				canvas.at<cv::Vec3b>(j, i)(1) = 0;
				canvas.at<cv::Vec3b>(j, i)(2) = 0;
			}
		}
	}
}

void Render2D::CanvasDraw(Vector2d center, double r, Vector3d color)
{
	double cell_x = VIEW_WIDTH / width;
	double cell_y = VIEW_HEIGHT / height;
	int cx = center(0) / cell_x;
	int cy = height - center(1) / cell_y;
	cv::circle(canvas, cv::Point(cx, cy), r, cv::Scalar(255*color(0), 255 * color(1), 255 * color(2)), -1);
}





Result Render2D::SceneDistance(double x, double y)
{
	Result l1 = { CircleSDF(x,y,270,-160,50),0.0f,0.0f,{ 2000.0f,2000.0f,2000.0f },BLACK };

	//Result l3 = { CircleSDF(x,y,lx2,ly2,50),0.0f,0.0f,{ 2000.0f,2000.0f,2000.0f },BLACK };
	//Result r6 = { sdf->Distance(Vector2d(x,y)).distance,0.3f,1.5f,BLACK,{ 0.002,0.2,0.002 } };

    Result r6 = { sdf->GridDistance(x,y),0.3f,1.5f,BLACK,{ 0.002,0.2,0.002 } };
	return UnionOp(l1, r6);
}

Color Render2D::Trace(double x, double y, double ix, double iy, int depth)
{
	double d = 0.01f;
	double sign = SceneDistance(x, y).d > 0 ? 1.0f : -1.0f;
	for (int i = 0; i < MAX_ITERATION&&d<MAX_DISTANCE; i++)
	{
		double px = x + ix*d, py = y + iy*d;
		Result e = SceneDistance(px, py);
		if (e.d * sign < EPSILON)
		{

			Color sum = e.emissive;
			if (depth<MAX_DEPTH && (e.reflectivity>0.0f || e.eta>0.0f))
			{
				double nx, ny, rx, ry, refl = e.reflectivity;
				Vector2d out;
				out = sdf->Gradient(px, py);
				nx = out[0];
				ny = out[1];
				nx *= sign; ny *= sign;
				if (e.eta > 0.0f)
				{
					if (Refract(ix, iy, nx, ny, sign < 0.0f ? e.eta : 1.0f / e.eta, &rx, &ry))
						sum = ColorAdd(sum, ColorScale(Trace(px - nx*BIAS, py - ny*BIAS, rx, ry, depth + 1), 1.0f - refl));
					else
						refl = 1.0f;
				}
				if (refl > 0.0f)
				{
					Reflect(ix, iy, nx, ny, &rx, &ry);
					sum = ColorAdd(sum, ColorScale(Trace(px + nx*BIAS, py + ny*BIAS, rx, ry, depth + 1), refl));
				}
			}
			if (sign < 0)
				return ColorMultiply(sum, BeerLambert(e.absorption, d));
			else
				return sum;
		}
		d += e.d*sign;
	}
	Color black = BLACK;
	return black;
}

void Render2D::PixelCalculate(int value)
{
	int i, j;
	i = value / canvas.cols;
	j = value%canvas.cols;
	Color energy = BLACK;
	for (int n = 0; n < N; n++)
	{
		double angle = 2 * M_PI / N * (n + (double)rand() / RAND_MAX);
		double rx = cos(angle), ry = sin(angle);
		energy = ColorAdd(energy, Trace(i, j, rx, ry, 0));
	}
	energy = ColorScale(energy, 1 / N);
	if (energy.r > 255) energy.r = 255;
	if (energy.g > 255) energy.g = 255;
	if (energy.b > 255) energy.b = 255;
	canvas.at<cv::Vec3b>(j, i)(0) = (int)energy.r;
	canvas.at<cv::Vec3b>(j, i)(1) = (int)energy.g;
	canvas.at<cv::Vec3b>(j, i)(2) = (int)energy.b;
}


Color ColorAdd(Color a, Color b)
{
	Color c = { a.r + b.r,a.g + b.g,a.b + b.b };
	return c;
}

Color ColorMultiply(Color a, Color b) {
	Color c = { a.r * b.r, a.g * b.g, a.b * b.b };
	return c;
}

Color ColorScale(Color a, double s) {
	Color c = { a.r * s, a.g * s, a.b * s };
	return c;
}

void Reflect(double ix, double iy, double nx, double ny, double *rx, double *ry)//矢量反射
{
	double p = ix*nx + iy*ny;
	*rx = ix - 2.0 * p*nx;
	*ry = iy - 2.0 * p*ny;
}


int Refract(double ix, double iy, double nx, double ny, double eta, double *rx, double *ry)
{
	double idotn = ix*nx + iy*ny;
	double k = 1.0f - eta*eta*(1 - idotn*idotn);
	if (k < 0.0f)
		return 0;
	double a = eta*idotn + sqrtf(k);
	*rx = eta*ix - a*nx;
	*ry = eta*iy - a*ny;
	return 1;
}

Color BeerLambert(Color a, double d)
{
	Color c = { expf(-a.r*d),expf(-a.g*d),expf(-a.b*d) };
	return c;
}

Result UnionOp(Result a, Result b)//并
{
	return a.d < b.d ? a : b;
}

Result IntersectOp(Result a, Result b)//交
{
	Result r = a;
	r.d = (a.d > b.d) ? a.d : b.d;
	return r;
}

Result SubtractOp(Result a, Result b)//差
{
	Result r = a;
	r.d = (a.d > -b.d) ? a.d : -b.d;
	return r;
}
