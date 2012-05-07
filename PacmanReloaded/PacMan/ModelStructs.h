#ifndef STRUCTS_
#define STRUCTS_

#include "Buffer.h"

using namespace std;

struct Material
{
	D3DXVECTOR3 m_ambient;
	D3DXVECTOR3 m_diffuse;
	D3DXVECTOR3 m_specular;
	int			m_shininess;
	float		m_alpha;
	ID3D10ShaderResourceView* m_textureResource;
};

struct Model
{
	D3DXVECTOR3 m_topBoundingCorner;
	D3DXVECTOR3 m_bottomBoundingCorner;
	Buffer*		m_vertexBuffer;
	Buffer*		m_indexBuffer;
	string		m_bufferName;
	UINT		m_offset;
	UINT		m_stride;
	int			m_size;
	Material*	m_material;
};

#endif