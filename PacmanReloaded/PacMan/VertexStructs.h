#ifndef VERTEXSTRUCTS_
#define VERTEXSTRUCTS_

struct Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
	D3DXVECTOR4 diff;
	D3DXVECTOR4 spec;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	D3DXVECTOR3		pos;
	float			pad1;
	D3DXVECTOR3		dir;
	float			pad2;      
	D3DXCOLOR		ambient;
	D3DXCOLOR		diffuse;
	D3DXCOLOR		specular;
	D3DXVECTOR3		att;
	float			spotPow;
	float			range;
};

#endif