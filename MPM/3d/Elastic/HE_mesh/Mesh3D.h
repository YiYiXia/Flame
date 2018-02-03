#pragma once

#include <vector>
#include <map>
#include "Vec.h"
#include"windows.h"
#include"Eigen/Dense"                                       //调用Eigen
#include <Eigen/Sparse>
using namespace Eigen;                                            //Eigen命名空间
typedef Eigen::SparseMatrix<double> SparseMatrixType;
// forward declarations of mesh classes
class HE_vert;
class HE_edge;
class HE_face;


using trimesh::point;

typedef trimesh::point point;
typedef trimesh::vec3  Vec3f;
typedef trimesh::vec4  Vec4f;

enum SelectTag
{
    UNSELECTED = 0,		//
    SELECTED,			//
	OTHER				//
};

enum BoundaryTag
{
	BOUNDARY,
	INNER,
	TO_SPLIT
};

/*!
*	The basic vertex class for half-edge structure.
*/
class HE_vert
{
public:


	int			id_;
	point 	position_;		//!< vertex position
	Vec3f	normal_;		//!< vertex normal
	Vec3f	texCoord_;		//!< texture coord
	Vec4f	color_;
	HE_edge		*pedge_;		//!< one of the half-edges_list emanating from the vertex
	int			degree_;
	BoundaryTag	boundary_flag_;	//!< boundary flag
	int			selected_;		//!< a tag: whether the vertex is selected
	int         bound;
	/*--------------------add by wang kang at 2013-10-12----------------------*/
	std::vector<size_t> neighborIdx;

public: 
	HE_vert(const Vec3f& v)
		: id_(-1), position_(v), pedge_(NULL), degree_(0), boundary_flag_(INNER), selected_(UNSELECTED)
		, color_(255.f / 255.f, 215.f / 255.f, 0.f/ 255.f, 1.f),neighborIdx()
	{}

	~HE_vert(void) { neighborIdx.clear();}

	bool		isOnBoundary(void) {return boundary_flag_==BOUNDARY;}
	int			id(void) {return id_;}
	Vec3f& normal(void) {return normal_;}
	Vec3f& position(void) {return position_;}
	Vec3f& texCoordinate(void) {return texCoord_;}
	Vec4f& color(void) {return color_;};
	int			degree(void) {return degree_;}
	int			selected(void) {return selected_;}
	BoundaryTag boundary_flag(void) {return boundary_flag_;}

	void		set_normal(const Vec3f& n) {normal_=n;}
	void		set_position(const Vec3f& p) {position_=p;}
	void		set_id(int id) {id_=id;}
	void		set_color(const Vec4f& c) {color_=c;}
	void		set_seleted(int tag){selected_=tag;}
	void		set_boundary_flag(BoundaryTag bt) {boundary_flag_=bt;}
};

/*!
*	The basic edge class for half-edge structure.
*/
class HE_edge
{
public:
	int			id_;
	HE_vert		*pvert_;		//!< vertex at the end of the half-edge
	HE_edge		*ppair_;		//!< oppositely oriented adjacent half-edge
	HE_face		*pface_;		//!< face the half-edge borders
	HE_edge		*pnext_;		//!< next half-edge around the face
	HE_edge		*pprev_;		//!< prev half-edge around the face
	Vec3f		texCoord_;		//!< texture coordinate of the end vertex
	BoundaryTag boundary_flag_;	//!< boundary flag
	int         bound;

public:
	HE_edge()
		: id_(-1), pvert_(NULL), ppair_(NULL)
		, pface_(NULL), pnext_(NULL), pprev_(NULL), boundary_flag_(INNER)
	{}

	~HE_edge()
	{}

	int		id(void) {return id_;}
	bool	isBoundary(void) {return boundary_flag_==BOUNDARY;}
	BoundaryTag boundary_flag(){return boundary_flag_;}
	void    set_boundary_flag(BoundaryTag bt) {boundary_flag_ = bt;}
};

/*!
*	The basic face class for half-edge structure.
*/
class HE_face
{
public:
	int			id_;
	HE_edge		*pedge_;		//!< one of the half-edges_list bordering the face
	Vec3f		normal_;		//!< face normal
	int			valence_;		//!< the number of edges_list
	int			selected_;		//!< a tag: whether the face is selected
	Vec4f		color_;			//!< the color of this face
	BoundaryTag boundary_flag_;	//!< this flag is used to split the mesh

public:
	HE_face()
		: id_(-1), pedge_(NULL), valence_(0), selected_(UNSELECTED), boundary_flag_(INNER)
	{}

	~HE_face()
	{}

	int				id(void) {return id_;}
	int				valence(void) {return valence_;}
	Vec3f&		normal(void) {return normal_;}
	int				selected(void) {return selected_;}
	Vec4f&		color(void) {return color_;}
	BoundaryTag		boundary_flag() {return boundary_flag_;}

	void			set_selected(int tag){selected_=tag;}
	void			set_color(const Vec4f& c) {color_=c;}
	void			set_boundary_flag(BoundaryTag bt) {boundary_flag_=bt;}

	/*-----------add by wang kang at 2013-10-13-------------*/
	void face_verts(std::vector<HE_vert *>& verts)
	{
		verts.resize(3);

		HE_edge* pedge = pedge_;
		verts[0] = pedge->pvert_;
		pedge = pedge->pnext_;
		verts[1] = pedge->pvert_;
		pedge = pedge->pnext_;
		verts[2] = pedge->pvert_;
	}
	point center() 
	{
		point center;
		HE_edge* pedge = pedge_;

		do 
		{
			center += pedge->pvert_->position();
			pedge = pedge->pnext_;

		} while (pedge != pedge_);

		center /= 3.0;
		return center;		
	}
public:

};

/*!
 
*/
class Mesh3D
{
	// type definitions
	typedef std::vector<HE_vert* >::iterator VERTEX_ITER;
	typedef std::vector<HE_face* >::iterator FACE_ITER;
	typedef std::vector<HE_edge* >::iterator EDGE_ITER;

	typedef std::vector<HE_vert* >::reverse_iterator VERTEX_RITER;
	typedef std::vector<HE_face* >::reverse_iterator FACE_RITER;
	typedef std::vector<HE_edge* >::reverse_iterator EDGE_RITER;
	typedef std::pair<HE_vert*, HE_vert* > PAIR_VERTEX;
public:
	std::vector<Vec3f> verty_list_;
	std::vector<Vec3f> temp_list_;

	bool pass;
	bool finish_prepare_;
	bool finish_open_;
	int turn;
public:
	// mesh data
	std::vector<HE_vert*>	*pvertices_list_;		//!< store vertex
	std::vector<HE_edge*>	*pedges_list_;		//!< store edges
	std::vector<HE_face*>	*pfaces_list_;		//!< store faces



	// mesh info
	int		num_components_;						//!< number of components
	float	average_edge_length_;				//!< the average edge length

	//! associate two end vertex with its edge: only useful in creating mesh
	std::map<std::pair<HE_vert*, HE_vert* >, HE_edge* >    edgemap_;	
	//std::map<std::pair<HE_vert*, HE_vert* >, HE_vert* >    midPointMap_;

	//! values for the bounding box
	float xmax_, xmin_, ymax_, ymin_, zmax_, zmin_;

public:
	//! constructor
	Mesh3D(void);

	//! destructor
	~Mesh3D(void);

	//! get the pointer of vertex list
	inline std::vector<HE_vert* >* get_vertex_list(void) {return pvertices_list_;}

	//! get the pointer of edges list
	inline std::vector<HE_edge* >* get_edges_list(void) {return pedges_list_;}

	//! get the pointer of faces list
	inline std::vector<HE_face* >* get_faces_list(void) {return pfaces_list_;}

	//! get the total number of vertex
	inline int num_of_vertex_list(void)	{return pvertices_list_ ? static_cast<int>(pvertices_list_->size()) : 0;}

	//! get the total number of half-edges
	inline int num_of_half_edges_list(void)	{return pedges_list_ ? static_cast<int>(pedges_list_->size()) : 0;}

	//! get the total number of edges
	inline int num_of_edge_list(void) {return num_of_half_edges_list()/2;}

	//! get the total number of faces
	inline int num_of_face_list(void) {return pfaces_list_ ? static_cast<int>(pfaces_list_->size()) : 0;}

	//! get the number of components
	inline int num_of_components(void) {return num_components_;}

	//! get the average edge length
	inline float average_edge_length(void) {return average_edge_length_;}

	//! get the pointer of the id-th vertex
	inline HE_vert* get_vertex(int id) {return id >= num_of_vertex_list() || id<0 ? NULL : (*pvertices_list_)[id];}

	//! get the pointer of the id-th half-edge
	inline HE_edge* get_half_edge(int id) {return id >= num_of_half_edges_list() || id<0 ? NULL : (*pedges_list_)[id];}

	//! get the pointer of the id-th face
	inline HE_face* get_face(int id) {return id >= num_of_face_list() || id<0 ? NULL : (*pfaces_list_)[id];}

	//! get the half-edge from vertex hv0 to hv1
	inline HE_edge* get_edge(HE_vert* hv0, HE_vert* hv1)
	{
		if (!hv0 || !hv1) return NULL;
		HE_edge* edge = hv0->pedge_;
		do 
		{
			if (edge->pvert_ == hv1)
			{
				return edge;
			}
			edge = edge->ppair_->pnext_;
		} while (edge!=hv0->pedge_);
		return NULL;
	}

	//! check whether the mesh id valid
	inline bool isValid(void)
	{
		if ( num_of_vertex_list()==0 || num_of_face_list()==0)
		{
			return false;
		}
		return true;
	}


	//! insert a vertex 
	/*!
	*	\param v a 3d point
	*	\return a pointer to the created vertex
	*/
	HE_vert* InsertVertex(const Vec3f& v);
	HE_vert* InsertVertexTemp(const Vec3f& v);
	//! insert an edge
	HE_edge* InsertEdge(HE_vert* vstart, HE_vert* vend);

	//! insert a face
	/*!
	*	\param vec_hv the vertex list of a face
	*	\return a pointer to the created face
	*/
	HE_face* InsertFace(std::vector<HE_vert* >& vec_hv);


	// FILE IO
	//! load a 3D mesh from an OBJ format file
	bool LoadFromOBJFile(const char* fins);
	//! export the current mesh to an OBJ format file
	void WriteToOBJFile(const char* fouts);

	//! update mesh:
	/*! 
	*	call it when you have created the mesh
	*/
	void UpdateMesh(void);

	//! update normal
	/*!
	*	compute all the normals of vertex and faces
	*/
	void UpdateNormal(void);

	//! compute the bounding box
	void ComputeBoundingBox(void);

	//! get the face with id0, id1, id2 vertices
	HE_face* get_face(int vId0, int vId1, int vId2);
	//! get the face whose vertices are the ids vertices
	HE_face* get_face(const std::vector<unsigned int>& ids);

	int		GetFaceId(HE_face* face);

	//! reset all the selected flags for the vertices as the tag
	void ResetVertexSelectedTags(int tag=UNSELECTED);
	//! reset all the selected flags for the faces as the tag
	void ResetFaceSelectedTags(int tag=UNSELECTED);

	//! check the vertex v0 and v1 whether are neighborhoods
	bool isNeighbors(HE_vert* v0, HE_vert* v1);

	//! get the selected "first" vertex, if no one is selected, return -1
	int GetSelectedVrtId();

	//! create a mesh by the vertices positions and triangle topology
	//!   the triIdx is defined as tri0_v0, tri0_v1, tri0_v2, tr1_v0, tr1_v1, tr1_v2, ...
	void CreateMesh(const std::vector<Vec3f>& verts, const std::vector<int>& triIdx);
	void CreateMesh(const std::vector<double>& verts, const std::vector<unsigned>& triIdx);

	int GetBoundaryVrtSize();
	//void divide();

	//void CreateMiniFace();
	void CopyVetry(const char*fins);
	void CopyVetry();
	void SetVetry();
	//void CalucateMatrix();
	void Create2D();
	void CreateBoundary();
	//void Test();
public:
	//! clear all the data
	void ClearData(void);
	//normal computation

	//! compute all the normals of faces
	void ComputeFaceslistNormal(void);
	//! compute the normal of a face
	void ComputePerFaceNormal(HE_face* hf);

	//! compute all the normals of vertex
	void ComputeVertexlistNormal(void);
	//! compute the normal of a vertex
	void ComputePerVertexNormal(HE_vert* hv); 

private:
	//! clear vertex
	void ClearVertex(void);
	void ClearVertexTemp(void);
	//! clear edges
	void ClearEdges(void);
	//! clear faces
	void ClearFaces(void);

	


	//! compute the number of components
	void ComputeNumComponents(void);

	//! compute the average edge length
	void ComputeAvarageEdgeLength(void);

	//! set vertex and edge boundary flag
	void SetBoundaryFlag(void);

	//! for the boundary vertices, make sure the half-edge structure can find all of them
	void BoundaryCheck();

	//! unify mesh
	void Unify(float size);

	//! check the face whether contains the vert
	bool isFaceContainVertex(HE_face* face, HE_vert* vert);


/*----------------------------------add by wang kang at 2013-10-12- -----------------------------------*/
	void get_neighborId(const size_t& vertid, std::vector<size_t>& neighbors)
	{
		HE_edge* edge = get_vertex(vertid)->pedge_;
		neighbors.clear();
		do 
		{
			neighbors.push_back(edge->pvert_->id());
			edge = edge->ppair_->pnext_;
		} while (edge!=get_vertex(vertid)->pedge_ && edge != NULL);

		std::vector<size_t> tmp = neighbors;
		for (size_t i=0; i!= tmp.size(); ++i)
			neighbors[i] = tmp[tmp.size()-i-1];
	}
private:
	void SetNeighbors()
	{
		for (size_t i=0; i!= num_of_vertex_list(); ++i)
			get_neighborId(i, pvertices_list_->at(i)->neighborIdx);
	}

public:
	void LinearTex()
	{
		for (size_t i=0; i != num_of_vertex_list(); ++i)
			pvertices_list_->at(i)->texCoord_ = pvertices_list_->at(i);
	}

	void SphereTex()
	{
		static const float pi = 3.1415926;
		for (size_t i=0; i != num_of_vertex_list(); ++i)
		{
			register HE_vert * vert = pvertices_list_->at(i);
			point p = vert->position().length()*vert->normal();


			float r = sqrt(p[1]*p[1]+p[0]*p[0]);

			vert->texCoord_[0] = asin(p[2])/pi + 0.5;
			vert->texCoord_[1] = acos(p[0])/(2*pi);
		}
	}
	/*---------------------------------------------------------*/

};
