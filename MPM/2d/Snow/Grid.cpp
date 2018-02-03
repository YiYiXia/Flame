#include"Grid.h"
#include"Parameters.h"
Grid::Grid(Vector2f pos, Vector2f dims, Vector2f cells,ParticleCloud* object)
{
	obj = object;
	start = pos;
	end = dims;
	cellsize[0] = (dims[0] - pos[0]) / cells[0];//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / cells[1];//格点竖直方向长度
	nodes_length = (cells[0] + 1)*(cells[1] + 1);
	size = cells + Vector2f(1,1);
	nodes = new GridNode[nodes_length];
	node_area = cellsize[0]*cellsize[1];
	
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
			nodes[(int)(j*size[0] + i)].position = start+Vector2f(cellsize[0] * i, cellsize[1] * j);
		}
	}
	//cout << cellsize[0] << "  " << cellsize[1] << endl;
	for (int i = 0; i < obj->size; i++)
	{
		Particle& p = obj->particles[i];
		p.grid_position[0] = (p.position[0] - start[0]) / cellsize[0];
		p.grid_position[1] = (p.position[1] - start[1]) / cellsize[1];
		float ox = p.grid_position[0], oy = p.grid_position[1];

	/*	if (ji >= 52)
		{
			cout << ji << "  " <<i<< "  " << p.position[0] << "  " << p.position[1] << endl << endl;
			cout << ji << "  " << i << "  " << (int)ox << "  " << (int)oy << endl << endl;
		}*/


		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			float y_pos = oy - y,wy = Grid::bspline(y_pos),dy = Grid::bsplineSlope(y_pos);
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				float x_pos = ox - x,wx = Grid::bspline(x_pos),dx = Grid::bsplineSlope(x_pos);
				float weight = wx*wy;
				p.weights[idx] = weight;
				p.weight_gradient[idx][0] = dx*wy;
				p.weight_gradient[idx][1] = wx*dy;
				p.weight_gradient[idx][0] /= cellsize[0];
				p.weight_gradient[idx][1] /= cellsize[1];
				/*cout << y<<"  "<<x<<"  "<< size[0] << endl;*/
				nodes[(int)(y*size[0] + x)].mass += weight*p.mass;
				
			}
		}
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
		Particle& p = obj->particles[i];
		int ox = p.grid_position[0],oy = p.grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				float w = p.weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					int n = (int)(y*size[0] + x);
					nodes[n].velocity += (p.velocity + 3 / (cellsize[0] * cellsize[1])*p.B*(nodes[n].position - p.position)) * w * p.mass;
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
		Particle& p = obj->particles[i];
		int ox = p.grid_position[0],
			oy = p.grid_position[1];
		//First compute particle density
		p.density = 0;
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				float w = p.weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					p.density += w * nodes[(int)(y*size[0] + x)].mass;
				}
			}
		}
		p.density /= node_area;
		p.volume = p.mass / p.density;
	}
	
}

void Grid::explicitVelocities(const Vector2f& gravity)
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
		Particle& p = obj->particles[i];
		Matrix2f energy = p.energyDerivative();
		/*if (ji >= 52)
		{
			cout << ji << "  "<<p.index << "  " << energy(0, 0) << "  " << energy(0, 1) << "  " << energy(1, 0) << "  " << energy(1, 1) << endl;
		}*/
		//cout << energy << endl<<endl;
		int ox = p.grid_position[0],
			oy = p.grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				float w = p.weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					int n = (int)(y*size[0] + x);
					
					nodes[n].velocity_new += energy*p.weight_gradient[idx];//体积在计算能量微分时已经乘上去了
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
			/*if (i == 78||i==89||i==100)
			{
				node.velocity_new = -0.5*node.velocity;
			}
			else
			{
				node.velocity_new = node.velocity + DT*(gravity - node.velocity_new / node.mass);
			}*/
			node.velocity_new = node.velocity + DT*(gravity - node.velocity_new / node.mass);
			/*if (ji >= 52)
			{
			cout << ji << "  " << i << "  " << node.velocity_new(0) << "  " << node.velocity_new(1) << endl ;
			}*/
			//cout << node.velocity[0] << "  " << node.velocity[1] << endl;
			//cout << node.mass << endl;
		}
			
	}
	collisionGrid();
}

void Grid::updateVelocities() const
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle& p = obj->particles[i];
		Matrix2f& grad = p.velocity_gradient;
		grad = Matrix2f::Zero();
		Matrix2f  affine = Matrix2f::Zero();
		Vector2f vp = Vector2f(0, 0);
		//VISUALIZATION PURPOSES ONLY:
		//Recompute density
		p.density = 0;
		p.velocity = Vector2f::Zero();
		int ox = p.grid_position[0],
			oy = p.grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++) {
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++) {
				float w = p.weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					GridNode &node = nodes[(int)(y*size[0] + x)];
					grad += node.velocity_new*p.weight_gradient[idx].transpose();
					p.density += w * node.mass;

					vp += node.velocity_new*w;
					affine = affine + w*node.velocity_new*(node.position - p.position).transpose();
					//p.velocity += w *node.velocity_new;
				}
			}
		}
		//VISUALIZATION: Update density
		p.density /= node_area;
		p.velocity = vp;
		p.B = affine;

		/*if (i % 24 == 0)
		{
			p.velocity = Vector2f(0, 0);
		}*/
	}
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
	Vector2f delta_scale = DT*Vector2f(1,1);
	delta_scale[0] /= cellsize[0];
	delta_scale[1] /= cellsize[1];
	for (int y = 0, idx = 0; y<size[1]; y++)
	{
		for (int x = 0; x<size[0]; x++, idx++)
		{
			GridNode &node = nodes[idx];
			if (node.active)
			{
				//Collision response
				//TODO: make this work for arbitrary collision geometry待办事项
				//这里不知为什么将cellsize在前面除了
				Vector2f new_pos = Vector2f(x, y);
				new_pos[0] += node.velocity_new[0] * delta_scale[0];
				new_pos[1] += node.velocity_new[1] * delta_scale[1];
				//Left border, right border
				if (new_pos[0] < BSPLINE_RADIUS )
				{
					if (node.velocity_new[0]<0)
					{
						/*node.velocity_new[0] *=0;
						node.velocity_new[1] *= STICKY;*/
						node.velocity_new = Vector2f::Zero();
					}
					//node.velocity_new[0] *= 0;
				}
				if (new_pos[0] > size[0] - BSPLINE_RADIUS-1 )
				{
					/*cout << node.velocity_new[0] << " " << node.velocity_new[1] << endl;*/
					if (node.velocity_new[0]>0)
					{
						/*node.velocity_new[0] *= 0;
						node.velocity_new[1] *= STICKY;*/
						node.velocity_new = Vector2f::Zero();
					}
					//node.velocity_new[0] *= 0;
				}
				//Bottom border, top border
				if (new_pos[1] < BSPLINE_RADIUS)
				{
					if (node.velocity_new[1]<0)
					{
						/*node.velocity_new[1] *= 0;
						node.velocity_new[0] *= STICKY;*/
						node.velocity_new = Vector2f::Zero();
					}
					//node.velocity_new[1] *= 0;
				}
				if (new_pos[1] > size[1] - BSPLINE_RADIUS-1)
				{
					if (node.velocity_new[1]>0)
					{
						/*node.velocity_new[1] *= 0;
						node.velocity_new[0] *= STICKY;*/
						node.velocity_new = Vector2f::Zero();
					}
					//node.velocity_new[1] *= 0;
				}
			}
		}
	}
}
void Grid::collisionParticles() const
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle& p = obj->particles[i];
		Vector2f new_pos = p.grid_position;
		new_pos[0] += DT*p.velocity[0] / cellsize[0];
		new_pos[1] += DT*p.velocity[1] / cellsize[1];
		//Left border, right border
		if (new_pos[0] < BSPLINE_RADIUS )
		{
			if (p.velocity[0] < 0)
			{
				p.velocity[0] *= 0;
				p.velocity[1] *= STICKY;
			}
			
		}
		if (new_pos[0] > size[0] - BSPLINE_RADIUS-1)
		{
			if (p.velocity[0] > 0)
			{
				p.velocity[0] *= 0;
				p.velocity[1] *= STICKY;
			}
		}
		if (new_pos[1] < BSPLINE_RADIUS )
		{
			if (p.velocity[1] < 0)
			{
				p.velocity[1] *= 0;
				p.velocity[0] *= STICKY;
			}
		}
			
		//Bottom border, top border
		if (new_pos[1] > size[1] - BSPLINE_RADIUS-1)
		{
			if (p.velocity[1] > 0)
			{
				p.velocity[1] *= 0;
				p.velocity[0] *= STICKY;
			}
		}
	}
}
