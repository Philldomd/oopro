#include "Terrain.h"

Terrain::Terrain(ID3D10Device* p_device, UINT p_width, UINT p_height, int p_Y)
{
	m_device = p_device;
	m_width = p_width;
	m_height = p_height;
	m_Y = p_Y;
}
Terrain::~Terrain()
{
	m_buffer->~Buffer();
	m_buffer = NULL;
	m_device = NULL;
}

void Terrain::initialize(float p_width, float p_height, D3DXVECTOR2 p_terrainScale)
{
	terrainVertex* initData = new terrainVertex[4];

	initData[0] = terrainVertex(D3DXVECTOR3(-p_terrainScale.x/2,-(float)m_Y,p_width-p_terrainScale.y/2), D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,0));
	initData[1] = terrainVertex(D3DXVECTOR3(-p_terrainScale.x/2,-(float)m_Y, -p_terrainScale.y/2), D3DXVECTOR3(0,1,0),D3DXVECTOR2(0,1));
	initData[2] = terrainVertex(D3DXVECTOR3(p_height-p_terrainScale.x/2, -(float)m_Y, p_width-p_terrainScale.y/2), D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,0));
	initData[3] = terrainVertex(D3DXVECTOR3(p_height-p_terrainScale.x/2, -(float)m_Y,  -p_terrainScale.y/2), D3DXVECTOR3(0,1,0),D3DXVECTOR2(1,1));

	m_groundWidth = D3DXVec3Length(&D3DXVECTOR3(D3DXVECTOR3(-p_terrainScale.x/2,-(float)m_Y,p_width-p_terrainScale.y/2) - D3DXVECTOR3(p_height-p_terrainScale.x/2, -(float)m_Y, p_width-p_terrainScale.y/2)));
	m_groundHeight = D3DXVec3Length(&D3DXVECTOR3(D3DXVECTOR3(-p_terrainScale.x/2,-(float)m_Y, -p_terrainScale.y/2) - D3DXVECTOR3(p_height-p_terrainScale.x/2, -(float)m_Y,  -p_terrainScale.y/2)));

	BUFFER_INIT_DESC bd;
	//Instance Buffer Description
	bd.ElementSize = sizeof( terrainVertex );
	bd.InitData = initData;
	bd.NumElements = 4;
	bd.Type = VERTEX_BUFFER;
	bd.Usage = BUFFER_DEFAULT;

	m_buffer = new Buffer();
	m_buffer->init(m_device, bd);

	//Delete init data
	SAFE_DELETE_ARRAY(initData);
}

void Terrain::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	m_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3DXMATRIX worldViewProj =  p_view * p_projection;
	
	fx::TerrainFX->setMatrix("g_mWorldViewProj", worldViewProj);

	m_buffer->apply(0);

	D3D10_TECHNIQUE_DESC techDesc;
	fx::TerrainFX->getTechnique()->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		fx::TerrainFX->apply(p);
		m_device->Draw( 4,0);
	}
}

UINT Terrain::getHeight()
{
	return m_height;
}

UINT Terrain::getWidth()
{
	return m_width;
}
int Terrain::getY()
{
	return m_Y;
}

float Terrain::getGroundHeight()
{
	return m_groundHeight;
}
float Terrain::getGroundWidth()
{
	return m_groundWidth;
}