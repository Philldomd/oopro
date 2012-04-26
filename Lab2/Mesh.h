#ifndef MESH_H
#define MESH_H

#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

using namespace std;

class Mesh
{
private:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;

		Vertex(D3DXVECTOR3 *pos, D3DXVECTOR2 *texC, D3DXVECTOR3 *norm)
		{
			position = *pos;
			texCoord = *texC;
			normal = *norm;
		}
	};
	vector<Vertex> m_vertexArray;
	Buffer* m_vertexBuffer;
	Shader* m_shader;
	ID3D10Device* m_d3dDevice;
	UINT m_nrVertices;
public:
	Mesh();
	~Mesh();

HRESULT Initialize(ID3D10Device* p_d3dDevice);
void AddVertex(D3DXVECTOR3 *p_pos, D3DXVECTOR2 *p_texC, D3DXVECTOR3 *p_norm);
void SetReserveVertexSize(long p_size);
void Clear();
void ShrinkToFit();
void PrepareToRender(D3DXMATRIX& p_mMVP, D3DXMATRIX& p_mWIT);
void Render();
void RenderSpecificShader(Shader* pShader, D3DXMATRIX& p_mLMVP);

};
#endif