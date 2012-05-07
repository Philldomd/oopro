#include "AABB.h"

AABB::AABB(D3DXVECTOR3 p_top, D3DXVECTOR3 p_bot, D3DXVECTOR4 p_color, ID3D10Device* device)
{
	m_top		= p_top;
	m_bottom	= p_bot;

	//m_index		= p_index;
	//mDevice = device;
	m_color		= p_color;
	m_type		= AABBOX;

	fulkod();
}
AABB::~AABB(){}

void AABB::calculateBounds()
{
	m_bounds[1] = D3DXVECTOR3( m_bounds[7].x,		m_bounds[0].y,		m_bounds[0].z ); // Xyz
	m_bounds[2] = D3DXVECTOR3( m_bounds[0].x,		m_bounds[7].y,		m_bounds[0].z ); // xYz
	m_bounds[3] = D3DXVECTOR3( m_bounds[7].x,		m_bounds[7].y,		m_bounds[0].z ); // XYz
	m_bounds[4] = D3DXVECTOR3( m_bounds[0].x,		m_bounds[0].y,		m_bounds[7].z ); // xyZ
	m_bounds[5] = D3DXVECTOR3( m_bounds[7].x,		m_bounds[0].y,		m_bounds[7].z ); // XyZ
	m_bounds[6] = D3DXVECTOR3( m_bounds[0].x,		m_bounds[7].y,		m_bounds[7].z ); // xYZ
	
	m_planes[NEARP] .setUpPlane( m_bounds[2], m_bounds[0], m_bounds[1] );
	m_planes[LEFT].  setUpPlane( m_bounds[4], m_bounds[0], m_bounds[2] );
	m_planes[BOTTOM].setUpPlane( m_bounds[4], m_bounds[0], m_bounds[1] );
	m_planes[RIGHT]. setUpPlane( m_bounds[3], m_bounds[1], m_bounds[5] );
	m_planes[FARP].	 setUpPlane( m_bounds[7], m_bounds[5], m_bounds[4] );
	m_planes[TOP].	 setUpPlane( m_bounds[3], m_bounds[2], m_bounds[6] );
}

void AABB::updatePosition(D3DXMATRIX p_scale, D3DXMATRIX p_translate)
{
	D3DXMATRIX scalate;
	D3DXMatrixMultiply( &scalate, &p_scale, &p_translate );

	D3DXVec3TransformCoord( &m_bounds[0], &m_bottom, &scalate );
	D3DXVec3TransformCoord( &m_bounds[7], &m_top, &scalate );

	calculateBounds();
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

void AABB::fulkod()
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
	desc.NumElements		= 36;
	desc.ElementSize		= sizeof(UINT);
	desc.Usage				= BUFFER_DEFAULT;
	desc.InitData			= temp;
	m_indexBuffer = new Buffer();
	m_indexBuffer->init(m_device, desc);

	m_shader = new Shader();

	const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_shader->init(m_device, "../PacMan/BoundingBox.fx", 12);

	D3D10_PASS_DESC PassDesc;
	m_shader->getTechniqueByName("DrawLine")->GetPassByIndex(0)->GetDesc(&PassDesc);
    m_device->CreateInputLayout(lineVertexLayout, 1, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &m_inputLayout);

	m_shader->setTechniqueByName("DrawLine");
	m_shader->setInputLayout(m_inputLayout);
}

void AABB::draw()
{
	D3DXMATRIX		WorldViewProj;
	D3DXMatrixIdentity(&WorldViewProj);

	m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	m_indexBuffer->apply(0);
	m_vertexBuffer->apply(0);

	D3D10_TECHNIQUE_DESC techDesc;
	m_shader->getTechnique()->GetDesc( &techDesc );
			
	m_shader->setFloat4("gColor", D3DXVECTOR4(1,0,1,1));
	m_shader->setMatrix("gWVP", WorldViewProj);

	m_shader->apply(0);
	m_device->DrawIndexed(24, 0, 0);
}