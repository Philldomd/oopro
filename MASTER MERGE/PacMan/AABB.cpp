#include "AABB.h"

AABB::AABB(D3DXVECTOR3 p_top, D3DXVECTOR3 p_bot, D3DXVECTOR4 p_color, ID3D10Device* device)
{
	m_top		= p_top;
	m_bottom	= p_bot;
	
	m_bounds[0] = m_bottom;
	m_bounds[7] = m_top;

	m_device	= device;
	m_color		= p_color;
	m_type		= AABBOX;
	debug		= true;

	initialize();
}
AABB::~AABB(){}

void AABB::initialize()
{
	calculateBounds();

	if (debug)
	{
		buildCubeIndices(0);

		BUFFER_INIT_DESC desc;
		desc.Type				= VERTEX_BUFFER;
		desc.NumElements		= 8;
		desc.ElementSize		= sizeof( D3DXVECTOR3 );
		desc.Usage				= BUFFER_DEFAULT;
		desc.InitData			= m_bounds;
		m_vertexBuffer = new Buffer();
		m_vertexBuffer->init(m_device, desc);

		int						temp[24];
		for (UINT i = 0; i < 24; i++)
		{
			temp[i]				= m_indices.at(i);
		}

		desc.Type				= INDEX_BUFFER;
		desc.NumElements		= 24;
		desc.ElementSize		= sizeof(UINT);
		desc.Usage				= BUFFER_DEFAULT;
		desc.InitData			= temp;

		m_indexBuffer = new Buffer();
		m_indexBuffer->init(m_device, desc);

		m_shader = new Shader();
		m_shader->init(m_device, "BoundingBox.fx", 12);
	}
}

void AABB::calculateBounds()
{
	m_bounds[1] = D3DXVECTOR3( m_bounds[7].x,		m_bounds[0].y,		m_bounds[0].z ); // Xyz
	m_bounds[2] = D3DXVECTOR3( m_bounds[0].x,		m_bounds[7].y,		m_bounds[0].z ); // xYz
	m_bounds[3] = D3DXVECTOR3( m_bounds[7].x,		m_bounds[7].y,		m_bounds[0].z ); // XYz
	m_bounds[4] = D3DXVECTOR3( m_bounds[0].x,		m_bounds[0].y,		m_bounds[7].z ); // xyZ
	m_bounds[5] = D3DXVECTOR3( m_bounds[7].x,		m_bounds[0].y,		m_bounds[7].z ); // XyZ
	m_bounds[6] = D3DXVECTOR3( m_bounds[0].x,		m_bounds[7].y,		m_bounds[7].z ); // xYZ
}

void AABB::updatePosition(D3DXMATRIX p_scale, D3DXMATRIX p_translate)
{
	D3DXMATRIX scalate;
	D3DXMatrixMultiply( &scalate, &p_scale, &p_translate );

	D3DXVec3TransformCoord( &m_bounds[0], &m_bottom, &scalate );
	D3DXVec3TransformCoord( &m_bounds[7], &m_top, &scalate );
}

void AABB::buildCubeIndices(int offset)
{
	// Cube Composition
	/*		  +----+  6    7	| Refered to as
			 /    /|			| indices of the
			+----+ |  4    5	| vertices below
	2	 3	|    | +			
			|    |/				
	0	 1	+----+								*/

	// Right/Left
	for( int i = 0; i < 8; i++ )
	{
		m_indices.push_back( i + offset );
	}
	// Froward/Backward
	for( int i = 0; i < 4; i++ )
	{
		m_indices.push_back( i + offset );
		m_indices.push_back( i + offset + 4 );
	}
	// Up/Down
	for( int i = 0; i < 2; i++ )
	{
		m_indices.push_back( i + offset );
		m_indices.push_back( i + offset + 2 );
		m_indices.push_back( i + offset + 4 );
		m_indices.push_back( i + offset + 2 + 4 );
	}
}

D3DXVECTOR3* AABB::getMax()
{
	return &m_bounds[7];
}

D3DXVECTOR3* AABB::getMin()
{
	return &m_bounds[0];
}

int AABB::boxVsBox( AABB* p_box )
{
	D3DXVECTOR3* max = p_box->getMax();
	D3DXVECTOR3* min = p_box->getMin();

	if ( getMin()->x > max->x || min->x > getMax()->x )
		return OUTSIDE;
	if ( getMin()->y > max->y || min->y > getMax()->y )
		return OUTSIDE;
	if ( getMin()->z > max->z || min->z > getMax()->z )
		return OUTSIDE;

	return INTERSECT;
}

void AABB::draw(D3DXMATRIX& p_world, D3DXMATRIX& p_view, D3DXMATRIX& p_proj)
{
	if (debug)
	{
		D3DXMATRIX WorldViewProj;
		D3DXMatrixMultiply( &WorldViewProj, &p_world, &p_view );
		D3DXMatrixMultiply( &WorldViewProj, &WorldViewProj, &p_proj );

		m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
		m_indexBuffer->apply(0);
		m_vertexBuffer->apply(0);

		D3D10_TECHNIQUE_DESC techDesc;
		m_shader->getTechnique()->GetDesc( &techDesc );
				
		m_shader->setFloat4("gColor", m_color);
		m_shader->setMatrix("g_mWorldViewProjection", WorldViewProj);

		m_shader->apply(0);
		m_device->DrawIndexed(24, 0, 0);
	}
}