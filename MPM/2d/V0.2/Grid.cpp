#include"Grid.h"

Grid::Grid(Vector2d pos, Vector2d dims, Vector2d cells,ParticleCloud* object)
{
	obj = object;
	start = pos;
	end = dims;
	cellsize[0] = (dims[0] - pos[0]) / cells[0];//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / cells[1];//格点竖直方向长度
	nodes_length = (cells[0] + 1)*(cells[1] + 1);
	size = cells + Vector2d(1,1);
	nodes = new GridNode[nodes_length];
	node_area = cellsize[0]*cellsize[1];
	
	
	//glass = new SDF();
	//Vector2d v(0.0, 0);
	//glass->Insert(2.05, 3.2,v);
	//glass->Insert(2.05, 3.95, v);
	//glass->Insert(2.8, 3.95, v);
	//glass->Insert(2.8, 3.2,v);


	//glass->Insert(0.5, 0.5, v);
	//glass->Insert(0.5, 4.5, v);
	//glass->Insert(4.5, 4.5, v);
	//glass->Insert(4.5, 0.5, v);
	//glass->Initialize();

	Time = 0;


	
}
Grid::~Grid()
{
	delete[] nodes;
}
void Grid::initializeMass()
{
	memset(nodes, 0, sizeof(GridNode)*nodes_length);
	
	//如果不绘制格点，就不要加这些
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			nodes[(int)(j*size[0] + i)].position = start+Vector2d(cellsize[0] * i, cellsize[1] * j);
		}
	}
	//cout << cellsize[0] << "  " << cellsize[1] << endl;
	for (int i = 0; i < obj->size; i++)
	{
		Particle* p = obj->particles[i];
		p->grid_position[0] = (p->position[0] - start[0]) / cellsize[0];
		p->grid_position[1] = (p->position[1] - start[1]) / cellsize[1];
		double ox = p->grid_position[0], oy = p->grid_position[1];
		/*cout << ox << "  " << oy<<"  " << i << endl;*/
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			double y_pos = oy - y,wy = Grid::bspline(y_pos),dy = Grid::bsplineSlope(y_pos);
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double x_pos = ox - x,wx = Grid::bspline(x_pos),dx = Grid::bsplineSlope(x_pos);
				double weight = wx*wy;
				p->weights[idx] = weight;
				p->weight_gradient[idx][0] = dx*wy;
				p->weight_gradient[idx][1] = wx*dy;
				p->weight_gradient[idx][0] /= cellsize[0];
				p->weight_gradient[idx][1] /= cellsize[1];

				/*cout << y<<"  "<<x<<"  "<< size[0] << endl;*/
				nodes[(int)(y*size[0] + x)].mass += weight*p->mass;
			}
		}
		//if (i == 21 && p.position[0]>3.5)
		//{
		//	cout << p.def_elastic << endl<<endl;
		//}
	}
	
	/*for (int i = 0; i < obj->particles.size(); i++)
	{
		cout << obj->particles[i].def_elastic << endl << endl;
	}*/


}

void Grid::initializeVelocities()
{
	for (int i = 0; i < obj->size; i++)
	{
		Particle* p = obj->particles[i];
		int ox = p->grid_position[0],oy = p->grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					int n = (int)(y*size[0] + x);
					nodes[n].velocity += (p->velocity +3 / (cellsize[0] * cellsize[1])*p->B*(nodes[n].position - p->position)) * w * p->mass;
					//nodes[n].velocity += (p.velocity) * w * p.mass;
					nodes[n].active = true;
				}
			}
		}
	}
	for (int i = 0; i<nodes_length; i++)
	{
		GridNode &node = nodes[i];
		if (node.active)
			node.velocity /= node.mass;
	}
	//collisionGrid();
}

void Grid::calculateVolumes() const
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle* p = obj->particles[i];
		if (p->volume_check == true)
		{
			continue;
		}
		p->volume_check == true;
		int ox = p->grid_position[0],
			oy = p->grid_position[1];
		//First compute particle density
		p->density = 0;
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					p->density += w * nodes[(int)(y*size[0] + x)].mass;
				}
			}
		}
		p->density /= node_area;
		p->volume = p->mass / p->density;
	}
	
}


void Grid::explicitVelocities(const Vector2d& gravity)
{
	/*for (int i = 0; i < nodes_length; i++)
	{
		GridNode &node = nodes[i];
		if (node.active)
		{
			cout << node.velocity[0] << "  " << node.velocity[1] << endl;
		}
		
	}*/

	for (int i = 0; i<obj->size; i++)
	{
		Particle* p = obj->particles[i];
		Matrix2d energy = p->energyDerivative();
		
		//cout << energy << endl<<endl;
		int ox = p->grid_position[0],
			oy = p->grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					int n = (int)(y*size[0] + x);
					
					nodes[n].velocity_new += energy*p->weight_gradient[idx];//体积在计算能量微分时已经乘上去了
				}
			}
		}
	}

	for (int i = 0; i<nodes_length; i++)
	{
		GridNode &node = nodes[i];
		//这个active起到一个很关键的作用，一来可以将计算放在有意义的格点上，二来可以避免mass为0时出现的无穷大
		if (node.active)
		{
			node.velocity_new = node.velocity + DT*(gravity - node.velocity_new / node.mass);
		}
			
	}
	collisionGrid();
}

void Grid::updateVelocities()
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle* p = obj->particles[i];
		Matrix2d& grad = p->velocity_gradient;
		grad = Matrix2d::Zero();
		Matrix2d  affine = Matrix2d::Zero();
		Vector2d vp = Vector2d(0, 0);
		//VISUALIZATION PURPOSES ONLY:
		//Recompute density
		p->density = 0;
		p->velocity = Vector2d::Zero();
		int ox = p->grid_position[0],
			oy = p->grid_position[1];
		//Vector2d temp = Vector2d::Zero();
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++) {
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++) {
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					GridNode &node = nodes[(int)(y*size[0] + x)];
					grad += node.velocity_new*p->weight_gradient[idx].transpose();
					p->density += w * node.mass;
				//	if (i == 21 && p.position[0]>3.5)
				//	{
				//		cout << node.velocity[0]<<" "<< node.velocity[1] <<endl;
				//	}
					//temp = temp + p.weight_gradient[idx];

					vp += node.velocity_new*w;
					affine = affine + w*node.velocity_new*(node.position - p->position).transpose();
					//p.velocity += w *node.velocity_new;
				}
			}
		}
		
		//VISUALIZATION: Update density
		p->density /= node_area;
		p->velocity = vp;
		p->B = affine;
		//if (i == 21 && p.position[0]>3.5)
		//{
			//cout << endl;
			//def_elastic = Matrix2d::Identity();
			//cout <<temp << endl<<endl;
		//}
		/*if (i == 222)
		{
			cout << p.velocity[0] << " " << p.velocity[1] << " " << p.position[0] << " " << p.position[1] << endl;
		}*/
		/*if (i == 400)
		{
			Matrix2d def_all = p.def_plastic;
			cout << def_all.determinant() << endl;
		}*/
		/*if (i % 24 == 0)
		{
			p.velocity = Vector2d(0, 0);

		}*/
	}
	Time = Time + DT;
	

	//int frame = 5000, bound = 4500;
	//if ((int)(Time / DT) % frame >=bound)
	//{
	//	glass->omega = M_PI / ((frame - bound)*DT);
	//	/*for (int i = 0; i < glass->vertices.size(); i++)
	//	{
	//		glass->velocity[i]= -glass->velocity[i];
	//	}*/
	//}
	//else
	//{
	//	glass->omega = 0;
	//}

	/*int frame = 600, bound = 300;
	if ((int)(Time / DT) % frame >= bound)
	{
		glass->v_center = Vector2d(0.5, 0);
	}
	else
	{
		glass->v_center = Vector2d(-0.5, 0);
	}
	glass->Update(DT);*/

	//collisionParticles();
}

void Grid::draw()
{
	glColor3f(1,0.1, 0.2);
	glPointSize(2);
	glBegin(GL_POINTS);
	
	for (int i = 0; i < nodes_length; i++)
	{
		if (i == 78 || i == 89 || i == 100)
		{
			glColor3f(0.1, 0.1, 0.2);
		}
		else
		{
			glColor3f(1, 0.1, 0.2);
		}
		glVertex2f(nodes[i].position[0], nodes[i].position[1]);

	}
	glEnd();
	
	for (int i = 0; i < nodes_length; i++)
	{
		glBegin(GL_LINES);
		glColor3f(0.1, 1, 0.2);

		glVertex2f(nodes[i].position[0], nodes[i].position[1]);
		glVertex2f(nodes[i].position[0]+0.01*nodes[i].velocity[0], nodes[i].position[1] + 0.01*nodes[i].velocity[1]);
		glEnd();
	}
	
}
void Grid::output()
{
	for (int i = 0; i < nodes_length; i++)
	{
		//cout << nodes[i].mass << endl;
		//cout << nodes[i].velocity[0]<<" "<< nodes[i].velocity[1] << endl;
	}
}

void Grid::collisionGrid()
{
	Vector2d delta_scale = DT*Vector2d(1,1);
	for (int y = 0, idx = 0; y<size[1]; y++)
	{
		for (int x = 0; x<size[0]; x++, idx++)
		{
			GridNode &node = nodes[idx];
			if (node.active)
			{
				Vector2d new_pos = node.position + DT*node.velocity_new;
				for (int i = 0; i < polygon.size(); i++)
				{
					int co;//用来修改内外
					if (polygon[i]->type == Boundary) co = 1;
					else if (polygon[i]->type == Object) co = -1;
					else continue;
					double d1 = co*polygon[i]->Distance(new_pos).distance;
					//double d2 = glass->Distance(node.position).distance;

					if (d1 > -0.05)
					{
						//node.velocity_new = Vector2d::Zero();
						Vector2d v_boundary = polygon[i]->SDFveloctiy(new_pos[0], new_pos[1]);
						//cout << v_boundary[0] << " " << v_boundary[1] << endl;
						Vector2d v_relate = node.velocity_new - v_boundary;
						Vector2d normal = -co*polygon[i]->Gradient(new_pos[0], new_pos[1]);
						/*if (v_relate.norm() > 90)
						{
						cout << y*size[0] + x << " " << v_relate[0] << "  " << v_relate[1] << endl;
						}*/
						double s = v_relate.dot(normal);
						if (s <= 0.0)
						{
							//Vector2d v_normal = s*normal;
							//v_relate = (v_relate - v_normal)*STICKY;
							if (normal[1]>0.95)
							{
								v_relate = Vector2d::Zero();
							}
							else
							{
								//cout << normal[0] << " " << normal[1] << endl;
								Vector2d v_normal = s*normal;
								//node.velocity_new = node.velocity_new - 2 * v_normal;
								v_relate = (v_relate - v_normal)*STICKY;
							}
							node.velocity_new = v_relate + v_boundary;

						}

					}
				}
				
			}
		}
	}
}
void Grid::collisionParticles() const
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle* p = obj->particles[i];
		Vector2d new_pos = p->position;
		//double d = glass->Distance(new_pos).distance;
		//if (d > -0.06)
		//{
		///*	p->Lambda = 1000.f;
		//	p->Mu = 1000.0f;*/
		//	//cout << 1 << endl;
		//	//p.velocity = Vector2d::Zero();
		//	//p.position=p.position
		//	//p.def_elastic = Matrix2d::Identity();
		//}
	}
}
