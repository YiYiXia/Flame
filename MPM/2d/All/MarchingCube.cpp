#include"MarchingCube.h"


MarchingCube::MarchingCube()
{

}
MarchingCube::~MarchingCube()
{

}

void MarchingCube::Polygon(cv::Mat p)
{
	
	int col = p.cols;
	int row = p.rows;
	double cell_x = VIEW_WIDTH / col;
	double cell_y = VIEW_HEIGHT / row;
	//Vector2d begin, end;
	//边线对应点
	map<int, Point*>::iterator it;
	//存放点
	Point* begin;
	Point* end;
	int num = 0;
	for (int i = 0; i < col-1; i++)
	{
		for (int j = 0; j < row-1; j++)
		{
			Vector2d p1(j, i);
			Vector2d p2(j, i + 1);
			Vector2d p3(j + 1, i + 1);
			Vector2d p4(j + 1, i);
			
			int t1 = 0, t2 = 0, t3 = 0, t4 = 0;

			if (p.at<cv::Vec3b>(j, i)(0) == 0) t1 = 0;
			else t1 = 1;
			if (p.at<cv::Vec3b>(j, i + 1)(0) == 0) t2 = 0;
			else t2 = 1;
			if (p.at<cv::Vec3b>(j + 1, i + 1)(0) == 0) t3 = 0;
			else t3 = 1;
			if (p.at<cv::Vec3b>(j + 1, i)(0) == 0) t4 = 0;
			else t4 = 1;
			bool key = true;
			int index;
			if (t1 + t2 == 1)
			{
				//cout << 1 << endl;
				Point* p;
				it = judge.find(j*(2 * col + 1) + i);
				if (it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p1 + p2) / 2);
					new_point->index = num;
					judge.insert(make_pair(j*(2 * col + 1) + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find(j*(2 * col + 1) + i);
				p = it->second;
				if (key == true)
				{
					begin = p;
					key = false;
				}
			}
			if (t2 + t3 == 1)
			{
				//cout << 2 << endl;
				Point* p;
				it = judge.find(j*(2 * col + 1) + col + 1 + i);
				if(it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p2 + p3) / 2);
					new_point->index = num;
					
					judge.insert(make_pair(j*(2 * col + 1) + col + 1 + i, new_point));
					//p = judge.find(j*(2 * col + 1) + col + 1 + i)->second;
					//cout << p->index << endl;
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find(j*(2 * col + 1) + col + 1 + i);
				p = it->second;
				if (key == true)
				{
					begin = p;
					key = false;
				}
				else
				{
					end = p;
					if (begin->p1 == NULL)	begin->p1 = end;
					else begin->p2 = end;
					if (end->p1 == NULL) end->p1 = begin;
					else end->p2 = begin;
					key = true;
				}
			}
			if (t3 + t4 == 1)
			{
				//cout << 3 << endl;
				Point* p;
				it = judge.find((j + 1)*(2 * col + 1) + i);
				//cout << p->c << endl;
				if (it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p3 + p4) / 2);
					new_point->index = num;
					judge.insert(make_pair((j + 1)*(2 * col + 1) + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find((j + 1)*(2 * col + 1) + i);
				p = it->second;
				
				//cout << 3 << " " << p->c << endl;
				if (key == true)
				{
					begin = p;
					key = false;
				}
				else
				{
					end = p;
					if (begin->p1 == NULL)	begin->p1 = end;
					else begin->p2 = end;
					if (end->p1 == NULL) end->p1 = begin;
					else end->p2 = begin;
					key = true;
				}
			}

			if (t4 + t1 == 1)
			{
				//cout << 4 << endl;
				Point* p;
				it = judge.find(j*(2 * col + 1) + col + i);
				if (it == judge.end())
				{
					Point* new_point;
					new_point = new Point((p4 + p1) / 2);
					new_point->index = num;
					judge.insert(make_pair(j*(2 * col + 1) + col + i, new_point));
					check.insert(make_pair(num, new_point));
					num++;
				}
				it = judge.find(j*(2 * col + 1) + col + i);
				p = it->second;
				if (key == true)
				{
					begin = p;
					key = false;
				}
				else
				{
					end = p;
					if (begin->p1 == NULL)	begin->p1 = end;
					else begin->p2 = end;
					if (end->p1 == NULL) end->p1 = begin;
					else end->p2 = begin;
					key = true;
				}
			}
		}
	}

	/*cout << check.size() << endl;
	for (it = check.begin(); it != check.end(); it++)
	{
		if (it->second->p1 == NULL || it->second->p2 == NULL) cout << "error" << endl;
		cout << it->second->index<<"  "<< it->second->p1->index <<"  "<< it->second->p2->index << endl;
		cout << it->second->c[0] << "  " << it->second->c[1] << endl;
	}*/
	Point* start;
	Point* p_now;
	Point* p_next;
	while (!check.empty())
	{
		
		SDF* sdf;
		sdf = new SDF();
		start = check.begin()->second;
		p_now = start;
		//cout << p_now->index << endl;
		p_next = p_now->p1;
		sdf->Insert(p_now->c[1], p_now->c[0]);
		check.erase(p_now->index);
		while (p_next != start)
		{
			
			//cout << p_now->index << endl;
			if (p_next->p1 == p_now)
			{
				p_now = p_next;
				p_next = p_next->p2;
				//cout << "p2" << endl;
			}
			else
			{
				p_now = p_next;
				p_next = p_next->p1;
				//cout << "p1" << endl;
			}
			sdf->Insert(p_now->c[1] ,  p_now->c[0] );
			check.erase(p_now->index);
		}
		//cout << 111111 << endl;
		polygons.push_back(*sdf);
		cout << sdf->vertices.size() << endl;
	}
}