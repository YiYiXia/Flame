#include"Render2D.h"

Render2D::Render2D(int h, int w)
	:height(h),width(w)
{
	canvas = cv::Mat::zeros(height, width, CV_8UC3);
}

Render2D::~Render2D()
{

}
void Render2D::CanvasClear()
{
	canvas = cv::Mat::zeros(height, width, CV_8UC3);
}
void Render2D::CanvasOutput(int num)
{
	string over = ".png";
	stringstream ss;
	string str;
	cout << num << endl;
	ss << num;
	ss >> str;
	str += over;
	cv::imwrite(str, canvas);
}
void Render2D::CanvasDraw(ParticleCloud *cloud)
{
	double cell_x = VIEW_WIDTH / width;
	double cell_y = VIEW_HEIGHT / height;
	//cout << cloud.particles.size() << endl;
	for (int i = 0; i < cloud->size; i++)
	{
		int cx = cloud->particles[i].position[0] / cell_x;
		int cy = height - cloud->particles[i].position[1] / cell_y;
		//cout << cx << " " << cy << endl;
		cv::circle(canvas, cv::Point(cx,cy),3 , cv::Scalar(255, 255, 255), -1);
	}
	cv::GaussianBlur(canvas, canvas, cv::Size(17, 17), 3, 3);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (canvas.at<cv::Vec3b>(j, i)(0) > 100)
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
void Render2D::CanvasShow()
{
	cv::imshow("1", canvas);
	cv::waitKey(0);
}