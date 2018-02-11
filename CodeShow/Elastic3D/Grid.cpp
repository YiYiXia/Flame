#include"Grid.h"

Grid::Grid(Vector3f pos, Vector3f dims, Vector3f cells,ParticleCloud* object)
{
	obj = object;
	start = pos;
	end = dims;
	cellsize[0] = (dims[0] - pos[0]) / cells[0];//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / cells[1];//格点竖直方向长度
	cellsize[2] = (dims[2] - pos[2]) / cells[2];
	nodes_length = (cells[0] + 1)*(cells[1] + 1)*(cells[2] + 1);
	size = cells + Vector3f(1,1,1);
	nodes = new GridNode[nodes_length];
	node_area = cellsize[0]*cellsize[1];
	
}
Grid::~Grid()
{
	delete[] nodes;
}

//将质量从粒子插值到格点
void Grid::initializeMass()
{
	memset(nodes, 0, sizeof(GridNode)*nodes_length);
	
	//如果不绘制格点，就不要加这些
	for (int i = 0; i < size[0]; i++)
		for (int j = 0; j < size[1]; j++)
			for (int k = 0; k < size[2]; k++)
				nodes[(int)(k*size[0] * size[1] + j*size[0] + i)].position = start + Vector3f(cellsize[0] * i, cellsize[1] * j, cellsize[2] * k);

	for (int i = 0; i < obj->size; i++)
	{
		Particle& p = obj->particles[i];
		p.grid_position[0] = (p.position[0] - start[0]) / cellsize[0];
		p.grid_position[1] = (p.position[1] - start[1]) / cellsize[1];
		p.grid_position[2] = (p.position[2] - start[2]) / cellsize[2];
		float ox = p.grid_position[0], oy = p.grid_position[1], oz = p.grid_position[2];
		//对粒子周围的格点进行遍历
		for (int idx = 0, z = oz - 1, z_end = z + 3; z <= z_end; z++)
		{
			float z_pos = oz - z, wz = Grid::bspline(z_pos), dz = Grid::bsplineSlope(z_pos);
			for (int y = oy - 1, y_end = y + 3; y <= y_end; y++)
			{
				float y_pos = oy - y, wy = Grid::bspline(y_pos), dy = Grid::bsplineSlope(y_pos);
				for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
				{
					float x_pos = ox - x, wx = Grid::bspline(x_pos), dx = Grid::bsplineSlope(x_pos);
					float weight = wx*wy*wz;
					p.weights[idx] = weight;
					p.weight_gradient[idx][0] = dx*wy*wz;
					p.weight_gradient[idx][1] = wx*dy*wz;
					p.weight_gradient[idx][2] = wx*wy*dz;
					p.weight_gradient[idx][0] /= cellsize[0];
					p.weight_gradient[idx][1] /= cellsize[1];
					p.weight_gradient[idx][2] /= cellsize[2];
					nodes[(int)(z*size[0] * size[1] + y*size[0] + x)].mass += weight*p.mass;
				}
			}
		}
		
	}

}


//将速度从粒子插值到格点
void Grid::initializeVelocities()
{
	for (int i = 0; i < obj->size; i++)
	{
		Particle& p = obj->particles[i];
		int ox = p.grid_position[0], oy = p.grid_position[1], oz = p.grid_position[2];
		for (int idx = 0, z = oz - 1, z_end = z + 3; z <= z_end; z++)
		{
			for (int y = oy - 1, y_end = y + 3; y <= y_end; y++)
			{
				for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
				{
					float w = p.weights[idx];
					if (w > BSPLINE_EPSILON)
					{
						int n = (int)(z*size[0] * size[1] + y*size[0] + x);
						nodes[n].velocity += (p.velocity + 3 / (cellsize[0] * cellsize[1])*p.B*(nodes[n].position - p.position)) * w * p.mass;
						nodes[n].active = true;
					}
				}
			}
		}
		
	}
	for (int i = 0; i<nodes_length; i++)
	{
		GridNode &node = nodes[i];
		if (node.active)
		{
			node.velocity /= node.mass;
		}
			
	}
}


//计算粒子的体积
void Grid::calculateVolumes() const
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle& p = obj->particles[i];
		int ox = p.grid_position[0],
			oy = p.grid_position[1],
			oz = p.grid_position[2];
		p.density = 0;
		for (int idx = 0, z = oz - 1, z_end = z + 3; z <= z_end; z++)
		{
			for (int y = oy - 1, y_end = y + 3; y <= y_end; y++)
			{
				for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
				{
					float w = p.weights[idx];
					if (w > BSPLINE_EPSILON)
					{
						p.density += w * nodes[(int)(z*size[0] * size[1] + y*size[0] + x)].mass;
					}
				}
			}
		}
		
		p.density /= node_area;
		p.volume = p.mass / p.density;
	}
	
}


//显式：计算格点受力，更新格点的速度
void Grid::explicitVelocities(const Vector3f& gravity)
{
	
	for (int i = 0; i<obj->size; i++)
	{
		Particle& p = obj->particles[i];
		Matrix3f energy = p.energyDerivative();
		int ox = p.grid_position[0],
			oy = p.grid_position[1],
			oz = p.grid_position[2];
		for (int idx = 0, z = oz - 1, z_end = z + 3; z <= z_end; z++)
		{
			for (int y = oy - 1, y_end = y + 3; y <= y_end; y++)
			{
				for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
				{
					float w = p.weights[idx];
					if (w > BSPLINE_EPSILON)//有一部分被筛选出来
					{
						int n = (int)(z*size[0] * size[1] + y*size[0] + x);

						nodes[n].velocity_new += energy*p.weight_gradient[idx];//体积在计算能量微分时已经乘上去了
					}
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
	collisionGrid();//碰撞检测
}


//更新粒子的速度，速度梯度，仿射矩阵
void Grid::updateVelocities() const
{
	for (int i = 0; i<obj->size; i++)
	{
		Particle& p = obj->particles[i];
		Matrix3f& grad = p.velocity_gradient;
		grad = Matrix3f::Zero();
		Matrix3f  affine = Matrix3f::Zero();//APIC的仿射矩阵
		Vector3f vp = Vector3f(0, 0, 0);
		p.density = 0;
		p.velocity = Vector3f::Zero();
		int ox = p.grid_position[0],
			oy = p.grid_position[1],
			oz = p.grid_position[2];
		for (int idx = 0, z = oz - 1, z_end = z + 3; z <= z_end; z++)
		{
			for (int y = oy - 1, y_end = y + 3; y <= y_end; y++)
			{
				for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
				{
					float w = p.weights[idx];
					if (w > BSPLINE_EPSILON)
					{
						GridNode &node = nodes[(int)(z*size[1] * size[0] + y*size[0] + x)];
						//Velocity gradient
						grad += node.velocity_new*p.weight_gradient[idx].transpose();
						p.density += w * node.mass;
						vp += w*node.velocity_new;
						affine = affine + w*node.velocity_new*(node.position - p.position).transpose();
					}
				}
			}
		}
		p.density /= node_area;
		p.velocity = vp;
		p.B = affine;
		
	}
}





//基于格点角度的碰撞检测，较为粗糙
void Grid::collisionGrid()
{
	Vector3f delta_scale = DT*Vector3f(1, 1, 1);
	delta_scale[0] /= cellsize[0];
	delta_scale[1] /= cellsize[1];
	delta_scale[2] /= cellsize[2];
	for (int z = 0, idx = 0; z < size[2]; z++)
	{
		for (int y = 0; y<size[1]; y++)
		{
			for (int x = 0; x<size[0]; x++, idx++)
			{
				GridNode &node = nodes[idx];
				if (node.active)
				{
					Vector3f new_pos = Vector3f(x, y, z);
					new_pos[0] += node.velocity_new[0] * delta_scale[0];
					new_pos[1] += node.velocity_new[1] * delta_scale[1];
					new_pos[2] += node.velocity_new[2] * delta_scale[2];
					//Left border, right border
					if (new_pos[0] < BSPLINE_RADIUS)
					{
						if (node.velocity_new[0]<0)
						{
							node.velocity_new[0] *= 0;
						}
					}
					if (new_pos[0] > size[0] - BSPLINE_RADIUS - 1)
					{
						/*cout << node.velocity_new[0] << " " << node.velocity_new[1] << endl;*/
						if (node.velocity_new[0]>0)
						{
							node.velocity_new[0] *= 0;
						}
					}
					//Bottom border, top border
					if (new_pos[1] < BSPLINE_RADIUS)
					{
						if (node.velocity_new[1]<0)
						{
							node.velocity_new[1] *= 0;
						}
					}
					if (new_pos[1] > size[1] - BSPLINE_RADIUS - 1)
					{
						if (node.velocity_new[1]>0)
						{
							node.velocity_new[1] *= 0;
						}
					}

					//Bottom border, top border
					if (new_pos[2] < BSPLINE_RADIUS)
					{
						if (node.velocity_new[2]<0)
						{
							node.velocity_new[2] *= 0;
						}
					}
					if (new_pos[2] > size[2] - BSPLINE_RADIUS - 1)
					{
						if (node.velocity_new[2]>0)
						{
							node.velocity_new[2] *= 0;
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
		Particle& p = obj->particles[i];
		Vector3f new_pos = p.grid_position;
		new_pos[0] += DT*p.velocity[0] / cellsize[0];
		new_pos[1] += DT*p.velocity[1] / cellsize[1];
		//Left border, right border
		if (new_pos[0] < BSPLINE_RADIUS )
		{
			if (p.velocity[0] < 0)
			{
				p.velocity[0] *= -1;
			}
			
		}
		if (new_pos[0] > size[0] - BSPLINE_RADIUS-1)
		{
			if (p.velocity[0] > 0)
			{
				p.velocity[0] *= -1;
			}
		}
		if (new_pos[1] < BSPLINE_RADIUS )
		{
			if (p.velocity[1] < 0)
			{
				p.velocity[1] *= -1;
			}
		}
			
		//Bottom border, top border
		if (new_pos[1] > size[1] - BSPLINE_RADIUS-1)
		{
			if (p.velocity[1] > 0)
			{
				p.velocity[1] *= -1;
			}
		}
	}
}
