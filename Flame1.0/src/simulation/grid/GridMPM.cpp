#include <Eigen/Sparse>
#include"../../../include/simulation/grid/GridMPM.h"
typedef Eigen::SparseMatrix<double> SparseMatrixType;

GridMPM::GridMPM(Vector2d pos, Vector2d dims, Vector2d cells,ParticleCloudMPM* object)
{
	obj = object;
	start = pos;
	end = dims;
	cellsize[0] = (dims[0] - pos[0]) / cells[0];//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / cells[1];//格点竖直方向长度
	nodes_length = (cells[0] + 1)*(cells[1] + 1);
	size = cells + Vector2d(1,1);
	nodes = new MPMGridNode[nodes_length];
	node_area = cellsize[0]*cellsize[1];
	Time = 0;
}

GridMPM::GridMPM(Vector2d pos, Vector2d dims, ParticleCloudMPM* object)
{
	obj = object;
	start = pos;
	end = dims;
	ParametersIn();
	cellsize[0] = (dims[0] - pos[0]) / (size[0] - 1);//格点横向长度
	cellsize[1] = (dims[1] - pos[1]) / (size[1] - 1);//格点竖直方向长度
	nodes_length = size[0] * size[1];
	nodes = new MPMGridNode[nodes_length];
	node_area = cellsize[0] * cellsize[1];
	Time = 0;
}

GridMPM::~GridMPM()
{
	delete[] nodes;
}
void GridMPM::InitializeMass()
{
	memset(nodes, 0, sizeof(MPMGridNode)*nodes_length);
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			nodes[(int)(j*size[0] + i)].position = start+Vector2d(cellsize[0] * i, cellsize[1] * j);
			nodes[(int)(j*size[0] + i)].index = (int)(j*size[0] + i);
		}
	}
	for (int i = 0; i < obj->size; i++)
	{
		ParticleMPM* p = obj->particles[i];
		p->grid_position[0] = (p->position[0] - start[0]) / cellsize[0];
		p->grid_position[1] = (p->position[1] - start[1]) / cellsize[1];
		double ox = p->grid_position[0], oy = p->grid_position[1];
		for (int idx = 0, y = (int)oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			double y_pos = oy - y,wy = BSpline(y_pos),dy = BSplineSlope(y_pos);
			for (int x = (int)ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				double x_pos = ox - x,wx = BSpline(x_pos),dx = BSplineSlope(x_pos);
				double weight = wx*wy;
				p->weights[idx] = weight;
				p->weight_gradient[idx][0] = dx*wy;
				p->weight_gradient[idx][1] = wx*dy;
				p->weight_gradient[idx][0] /= cellsize[0];
				p->weight_gradient[idx][1] /= cellsize[1];
				nodes[(int)(y*size[0] + x)].mass += weight*p->mass;
			}
		}
	}
}

void GridMPM::InitializeVelocities()
{
	for (int i = 0; i < obj->size; i++)
	{
		ParticleMPM* p = obj->particles[i];
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
					nodes[n].active = true;
				}
			}
		}
	}
	for (int i = 0; i<nodes_length; i++)
	{
		MPMGridNode &node = nodes[i];
		if (node.active)
		{
			node.velocity /= node.mass;
			//cout << node.mass <<" "<< node.velocity[0] << " " << node.velocity[1] << endl;
		}	
	}
}

void GridMPM::CalculateVolumes() const
{
	for (int i = 0; i<obj->size; i++)
	{
		ParticleMPM* p = obj->particles[i];
		if (p->volume_check == true)
		{
			continue;
		}
		p->volume_check == true;
		int ox = p->grid_position[0],
			oy = p->grid_position[1];
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


void GridMPM::ExplicitForce()
{
	for (int i = 0; i<obj->size; i++)
	{
		ParticleMPM* p = obj->particles[i];
		Matrix2d energy = p->EnergyDerivative();
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
					nodes[n].force += energy*p->weight_gradient[idx];//体积在计算能量微分时已经乘上去了
				}
			}
		}
	}

}

void GridMPM::GridVelocityUpdate()
{
	for (int i = 0; i<nodes_length; i++)
	{
		MPMGridNode &node = nodes[i];
		//这个active起到一个很关键的作用，一来可以将计算放在有意义的格点上，二来可以避免mass为0时出现的无穷大
		if (node.active)
		{
			node.velocity_new = node.velocity - DT*(node.force / node.mass);
		}
	}

}

void GridMPM::ApplyExternForce()
{
	for (int i = 0; i<nodes_length; i++)
	{
		MPMGridNode &node = nodes[i];
		//这个active起到一个很关键的作用，一来可以将计算放在有意义的格点上，二来可以避免mass为0时出现的无穷大
		if (node.active)
		{
			node.velocity_new = node.velocity_new + DT*gravity;
		}
	}
}

void GridMPM::UpdateVelocities()
{
	for (int i = 0; i<obj->size; i++)
	{
		ParticleMPM* p = obj->particles[i];
		Matrix2d& grad = p->velocity_gradient;
		grad = Matrix2d::Zero();
		Matrix2d  affine = Matrix2d::Zero();
		Vector2d vp = Vector2d(0, 0);
		p->density = 0;
		p->velocity = Vector2d::Zero();
		int ox = p->grid_position[0],
			oy = p->grid_position[1];
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++) {
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++) {
				double w = p->weights[idx];
				if (w > BSPLINE_EPSILON)
				{
					MPMGridNode &node = nodes[(int)(y*size[0] + x)];
					grad += node.velocity_out*p->weight_gradient[idx].transpose();
					p->density += w * node.mass;
					vp += node.velocity_out*w;
					affine = affine + w*node.velocity_out*(node.position - p->position).transpose();
				}
			}
		}
		p->density /= node_area;
		p->velocity = vp;
		p->B = affine;
	}
	Time = Time + DT;
}

void GridMPM::CollisionGrid()
{
	Vector2d delta_scale = DT*Vector2d(1,1);
	for (int y = 0, idx = 0; y<size[1]; y++)
	{
		for (int x = 0; x<size[0]; x++, idx++)
		{
			MPMGridNode &node = nodes[idx];
			if (node.active)
			{
				node.velocity_out = node.velocity_new;
				
				Vector2d new_pos = node.position + DT*node.velocity_new;
				for (int i = 0; i < polygon.size(); i++)
				{
					int co;//用来修改内外
					if (polygon[i]->type == Boundary) co = 1;
					else if (polygon[i]->type == Object|| polygon[i]->type == Rivet) co = -1;
					else continue;//汇源不做碰撞处理
					double d1 = co*polygon[i]->Distance(new_pos).distance;
					double d2 = co*polygon[i]->Distance(node.position).distance;
					//cout << polygon.size() << endl;
					if (d1 > -0.05)
					{
						//cout << "yes" << endl;
						Vector2d v_boundary = polygon[i]->SDFveloctiy(new_pos[0], new_pos[1]);
						Vector2d v_relate = node.velocity_new - v_boundary;
						if (polygon[i]->type == Rivet)
						{
							v_relate = Vector2d::Zero();
							node.velocity_out = v_relate + v_boundary;
							//cout << node.velocity_out << endl;
							continue;
						}
						Vector2d normal = -co*polygon[i]->Gradient(new_pos[0], new_pos[1]);
						double s = v_relate.dot(normal);
						if (s <= 0.0)
						{
							node.collision = true;
							Vector2d v_normal = s*normal;
							v_relate = (v_relate - v_normal);
							node.velocity_out = v_relate + v_boundary;
							node.normal = normal;
						}
					}
				}		
			}
		}
	}
}

void GridMPM::Friction()
{
	for (int y = 0, idx = 0; y < size[1]; y++)
	{
		for (int x = 0; x < size[0]; x++, idx++)
		{
			MPMGridNode &node = nodes[idx];
			if (node.active)
			{
				if (node.collision == true)
				{
					Vector2d new_pos = node.position + DT*node.velocity_new;
					Vector2d normal = node.normal;
					Vector2d v_tangent = node.velocity_out - normal*(normal.dot(node.velocity_out));
					if (v_tangent.norm() < 1e-12) continue;
					double delta_v = (node.velocity_out - node.velocity_new).norm();
					double v_t = v_tangent.norm();
					Vector2d tangent = v_tangent / v_t;
					node.velocity_out = node.velocity_out - ((v_t < Friction_c*delta_v) ? v_t : Friction_c*delta_v)*tangent;
				}
			}
		}
	}
}

void GridMPM::ImplicitVelocities()
{
	double beta = 0, alpha = 0, gama = 0, div = 0;
	//此处为简化代码，如此写了
	for (int idx = 0; idx<nodes_length; idx++)
	{
		MPMGridNode& n = nodes[idx];
		n.imp_active = n.active;
		if (n.imp_active)
		{
			n.p = n.velocity_new;
			n.x = n.velocity_new;
		}
	}
	//初始r,p
	RecomputeImplicitForces();
	for (int idx = 0; idx<nodes_length; idx++)
	{
		MPMGridNode& n = nodes[idx];
		n.imp_active = n.active;
		if (n.imp_active)
		{
			n.r = n.velocity_new - n.Ls;
			n.p = n.r;
		}
	}
	while (1)
	{
		RecomputeImplicitForces();
		alpha = 0;
		div = 0;
		gama = 0;
		for (int idx = 0; idx<nodes_length; idx++)
		{
			MPMGridNode& n = nodes[idx];
			n.imp_active = n.active;
			if (n.imp_active)
			{
				gama += n.r.dot(n.r);
				div += n.p.dot(n.Ls);
			}
		}
		if (div < 1e-15) break;
		alpha = gama / div;
		for (int idx = 0; idx<nodes_length; idx++)
		{
			MPMGridNode& n = nodes[idx];
			n.imp_active = n.active;
			if (n.imp_active)
			{
				n.x = n.x + alpha*n.p;
				n.r_new = alpha*n.Ls;
			}
		}
		double judge = 0;
		for (int idx = 0; idx<nodes_length; idx++)
		{
			MPMGridNode& n = nodes[idx];
			n.imp_active = n.active;
			if (n.imp_active)
			{
				judge += n.r_new.dot(n.r_new);
			}
		}
		if (judge < 1e-12) break;
		beta = judge / gama;
		for (int idx = 0; idx<nodes_length; idx++)
		{
			MPMGridNode& n = nodes[idx];
			n.imp_active = n.active;
			if (n.imp_active)
			{
				n.p = n.r_new + beta*n.p;
				n.r = n.r_new;
			}
		}

	}

	for (int idx = 0; idx<nodes_length; idx++)
	{
		MPMGridNode& n = nodes[idx];
		n.imp_active = n.active;
		if (n.imp_active)
		{
			n.velocity_new = n.x;
		}
	}
}
void GridMPM::RecomputeImplicitForces()
{
	//Vector2d temp;
	//int key = 0;
	for (int idx = 0; idx<nodes_length; idx++)
	{
		MPMGridNode& n = nodes[idx];
		if (n.imp_active)
		{
			n.Im_force = Vector2d::Zero();
			//cout << n.force[0] << " " << n.force[1] << endl;
		}
	}
	for (int i = 0; i<obj->size; i++)
	{
		ParticleMPM* p = obj->particles[i];
		p->delta_v = Matrix2d::Zero();
		int ox = p->grid_position[0],
			oy = p->grid_position[1];
		//计算p上的delta_v
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				MPMGridNode& n = nodes[(int)(y*size[0] + x)];
				if (n.imp_active)
				{
					p->delta_v = p->delta_v + n.p*p->weight_gradient[idx].transpose();
				}
			}
		}
		p->DeltaForce();//计算Ap，即delta_f
		for (int idx = 0, y = oy - 1, y_end = y + 3; y <= y_end; y++)
		{
			for (int x = ox - 1, x_end = x + 3; x <= x_end; x++, idx++)
			{
				MPMGridNode& n = nodes[(int)(y*size[0] + x)];
				n.Im_force += p->delta_f*p->weight_gradient[idx];//粒子p对周围格点隐式力的贡献
			}
		}
	}

	for (int idx = 0; idx<nodes_length; idx++)
	{
		MPMGridNode& n = nodes[idx];
		if (n.imp_active)
		{
			n.Ls = n.p + 1.0*DT / n.mass*n.Im_force;
			//cout << n.force[0] << " " << n.force[1] << endl;
		}



	}
}

void GridMPM::ParticleCheck()
{
	obj->particles.erase(std::remove_if(obj->particles.begin(), obj->particles.end(), [&](ParticleMPM* & p)
	{
		//cout << "check" << endl;
		int co;
		Vector2d pro = p->position;
		for (int j = 0; j < polygon.size(); j++)
		{
			//cout << "poly"<<" "<< polygon.size() << endl;
			if (polygon[j]->type == Sink) co = 1;
			else continue;
			//cout << "polypoly" << endl;
			double d1 = co*polygon[j]->Distance(pro).distance;

			if (d1 > -0.05)
			{
				free(p);
				//cout << "in" << endl;
				return true;
			}
			else
			{
				//cout << "out" << endl;
				return false;
			}
		}
		return false;
	}), obj->particles.end());
	obj->size = obj->particles.size();
}

void GridMPM::ParametersIn()
{
	ifstream infile;
	infile.open("config/GridConfig.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	string st;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("Resolution_x") == 0) {
			infile >> val;
			size[0] = val + 1;
		}
		if (st.compare("Resolution_y") == 0) {
			infile >> val;
			size[1] = val + 1;
		}
		if (st.compare("Friction") == 0) {
			infile >> val;
			Friction_c = val;
		}
		infile >> st;
	}
}

void GridMPM::GridCheck()//方程式解法的初始化
{
	active_length = 0;
	active_list.clear();
	for (int y = 0, idx = 0; y < size[1]; y++)
	{
		for (int x = 0; x < size[0]; x++, idx++)
		{
			MPMGridNode &node = nodes[idx];
			Vector2d new_pos = node.position;
			for (int i = 0; i < polygon.size(); i++)
			{
				int co;//用来修改内外
				if (polygon[i]->type == Boundary) co = 1;
				else if (polygon[i]->type == Object || polygon[i]->type == Rivet) co = -1;
				else continue;//汇源不做碰撞处理
				double d1 = co*polygon[i]->Distance(new_pos).distance;
				if (d1 > -0.05)
				{
					node.type = BOUNDARY;
				}
				else if (node.active)
				{
					node.type = LIQUID;
					node.s_index = active_length;
					active_list.push_back(&node);
					active_length++;
				}
				else
				{
					node.type = EMPTY;
				}
			}
		}
	}
}

//求解p，相当于连续介质中的A
void GridMPM::SolvePressure()
{
	typedef Eigen::Triplet<double> T;
	std::vector<T> tripletList;
	SparseMatrixType M(active_length, active_length);                                                   //矩阵
	VectorXd X(active_length);                                                           //右方系数
	VectorXd Y(active_length); 
	double scale = DT / (cellsize[0] * cellsize[0]);
	//默认active的地方都是liquid
	//边界先不动
	int mark;
	for (int i = 0; i < active_list.size(); i++)
	{
		double major = 0;
		double right = 0;
		//左
		mark = active_list[i]->index - 1;
		if (nodes[mark].type == LIQUID)
		{
			
			major += 1;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
			//这里要考虑velocity的更新
			right += -(active_list[i]->velocity_new[0] - nodes[mark].velocity_new[0]) / (2 * cellsize[0]);
			//cout << "l" << endl;
		}
		else if (nodes[mark].type == EMPTY)
		{
			major += 1;
			//right += -(active_list[i]->velocity_new[0] - nodes[mark].velocity_new[0]) / (2 * cellsize[0]);
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			right += -(active_list[i]->velocity_new[0] ) / ( 2 *cellsize[0]);
		//	right += (-nodes[mark].velocity[0]) / cellsize[0];//此处之后要加边界速度，如此除是否可以？
		}

		//右
		mark = active_list[i]->index + 1;
		if (nodes[mark].type == LIQUID)
		{
			major += 1;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
			//这里要考虑velocity的更新
			right += -(-active_list[i]->velocity_new[0] + nodes[mark].velocity_new[0]) / (2 * cellsize[0]);
			//cout << "r" << endl;
		}
		else if (nodes[mark].type == EMPTY)
		{
			major += 1;
			//right += -(-active_list[i]->velocity_new[0] + nodes[mark].velocity_new[0]) / (2 * cellsize[0]);
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			right += -(-active_list[i]->velocity_new[0] ) / (2 * cellsize[0]);
		}

		//上
		mark = active_list[i]->index + size[0];
		if (nodes[mark].type == LIQUID)
		{
			major += 1;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
			//这里要考虑velocity的更新
			right += -(-active_list[i]->velocity_new[1] + nodes[mark].velocity_new[1]) / (2 * cellsize[0]);
			//cout << "u" << right << endl;
		}
		else if (nodes[mark].type == EMPTY)
		{
			major += 1;
			//right += -(-active_list[i]->velocity_new[1] + nodes[mark].velocity_new[1]) / (2 * cellsize[0]);
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			right += -(-active_list[i]->velocity_new[1]) / (2 * cellsize[0]);
		}

		//下
		mark = active_list[i]->index - size[0];
		if (nodes[mark].type == LIQUID)
		{
			//cout << "d" << right << endl;
			major += 1;
			tripletList.push_back(T(i, nodes[mark].s_index, -scale));
			//这里要考虑velocity的更新
			right += -(active_list[i]->velocity_new[1] - nodes[mark].velocity_new[1]) / (2 * cellsize[0]);
			//cout << "d" << -(active_list[i]->velocity_new[1] - nodes[mark].velocity_new[1]) / (2 * cellsize[0]) << endl;
			//cout << "d" << right << endl;
		}
		else if (nodes[mark].type == EMPTY)
		{
			major += 1;
			//right += -(active_list[i]->velocity_new[1] - nodes[mark].velocity_new[1]) / (2 * cellsize[0]);
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			right += -(active_list[i]->velocity_new[1]) / (2 * cellsize[0]);
		}


		tripletList.push_back(T(i, i, major*scale));
		Y(i) = right;
		//cout << "round" << endl;
	}
	
	M.setFromTriplets(tripletList.begin(), tripletList.end());
	//cout << Y << endl << endl;
	SparseLU<SparseMatrixType>MatricesM;
	MatricesM.compute(M);                                                              //预分解
	X = MatricesM.solve(Y);
	//cout << X(100) << endl;
	for (int i = 0; i < active_list.size(); i++)
	{
		active_list[i]->pressure = X(i);
	}
	
}


void GridMPM::SolveUpdate()
{
	double scale;
	scale = DT / (2 * cellsize[0]);
	int mark;

	for (int i = 0; i < active_list.size(); i++)
	{

		double p_x = 0, p_y = 0;

		mark = active_list[i]->index - 1;
		if (nodes[mark].type == LIQUID)
		{
			p_x += (-active_list[i]->pressure + nodes[mark].pressure)*scale;
		}
		else if (nodes[mark].type == EMPTY)
		{
			//p_x += -active_list[i]->pressure*scale;
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			//p_x += -2*active_list[i]->pressure*scale;
			active_list[i]->velocity_out[0] = 0;
			continue;
		}

		mark = active_list[i]->index + 1;
		if (nodes[mark].type == LIQUID)
		{
			p_x += (active_list[i]->pressure - nodes[mark].pressure)*scale;
		}
		else if (nodes[mark].type == EMPTY)
		{
			//p_x += active_list[i]->pressure*scale;
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			//p_x += 2 * active_list[i]->pressure*scale;
			active_list[i]->velocity_out[0] = 0;
			continue;
		}

		mark = active_list[i]->index - size[0];
		if (nodes[mark].type == LIQUID)
		{
			p_y += (-active_list[i]->pressure + nodes[mark].pressure)*scale;
		}
		else if (nodes[mark].type == EMPTY)
		{
			//p_y += -active_list[i]->pressure*scale;
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			//p_y += -2 * active_list[i]->pressure*scale;
			active_list[i]->velocity_out[1] = 0;
			continue;
		}

		mark = active_list[i]->index + size[0];
		if (nodes[mark].type == LIQUID)
		{
			p_y += (active_list[i]->pressure - nodes[mark].pressure)*scale;
		}
		else if (nodes[mark].type == EMPTY)
		{
			//p_y += active_list[i]->pressure*scale;
		}
		else if (nodes[mark].type == BOUNDARY)
		{
			//p_y += 2 * active_list[i]->pressure*scale;
			active_list[i]->velocity_out[1] = 0;
			continue;
		}

		active_list[i]->velocity_out[0] = active_list[i]->velocity_new[0] + p_x;
		active_list[i]->velocity_out[1] = active_list[i]->velocity_new[1] + p_y;
	}
}