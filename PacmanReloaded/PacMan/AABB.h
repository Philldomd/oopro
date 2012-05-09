#ifndef AABB_H
#define AABB_H

#include "Buffer.h"
#include "Shader.h"
#include "stdafx.h"
#include "BoundingVolume.h"

struct Plane
{
	// p = ax + by + cz or p = a dot r where p is a point in the plane.
	// a is a vector pointing at a point in the plane, r is pointing at 
	// another and r - a is perpendicular to the plane normal.
	// Which means the plane could be described as n dot ( r - a ) = 0.

	D3DXVECTOR3		normal;
	D3DXVECTOR3		a;
	float			distance; // lenght of vector a.
	
	Plane(){}
	Plane( D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3){ setUpPlane(v1, v2, v3); }
	void setUpPlane( D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3 )
	{
		D3DXVECTOR3 v, u;

		v = v1 - v2;
		u = v3 - v2;

		D3DXVec3Cross(&normal, &u, &v);
		D3DXVec3Normalize(&normal, &normal);
		a = v2;

		distance = - D3DXVec3Dot( &normal, &a );
	}

	float calculateDistance( D3DXVECTOR3* v )
	{
		return ( distance + D3DXVec3Dot( &normal, v ) );
	}
};

class AABB : public BoundingVolume
{
public:
	static enum { OUTSIDE = 0, INTERSECT, INSIDE };

	AABB( D3DXVECTOR3 p_top, D3DXVECTOR3 p_bot, D3DXVECTOR4 p_color, ID3D10Device* device );
	~AABB();

	void				calculateBounds();
	void				draw( D3DXMATRIX& p_world, D3DXMATRIX& p_view, D3DXMATRIX& p_proj );
	void				buildCubeIndices( int offset );
	void				updatePosition( D3DXMATRIX p_scale, D3DXMATRIX p_translate );
	void				initialize();
	D3DXVECTOR3*		getMax();
	D3DXVECTOR3*		getMin();
	int					boxVsBox( AABB* p_box );

private:
	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

	bool				debug;

	D3DXVECTOR3			m_bottom;
	D3DXVECTOR3			m_top;
	D3DXVECTOR3			m_bounds[8];
	D3DXVECTOR4			m_color;
	std::vector<int>	m_indices;
	Plane				m_planes[6];
	
	ID3D10Device*		m_device;
	Buffer*				m_indexBuffer;
	Buffer*				m_vertexBuffer;
	Shader*				m_shader;
	ID3D10InputLayout*	m_inputLayout;
};

#endif