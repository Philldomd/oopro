#ifndef GROUND_H
#define GROUND_H

#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

class Ground
{
public:
	Ground();
	~Ground();

	HRESULT Initialize(ID3D10Device* p_d3dDevice);
	void PrepareToRender(D3DXMATRIX& p_mMVP, D3DXMATRIX& p_mLMVP, D3DXVECTOR2& pSMAPSize);
	void Render();
	void RenderSpecificShader(Shader* pShader, D3DXMATRIX& p_mLMVP);
	void SetShadowMap(ID3D10ShaderResourceView* pTex);
	void SetGroundTexNULL();
	ID3D10ShaderResourceView * getShaderResource();
private:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
		Vertex(D3DXVECTOR3 pos, D3DXVECTOR2 texC)
		{
			position = pos;
			texCoord = texC;
		}
	};
	ID3D10ShaderResourceView *pSRView;
	Buffer* m_vertexBuffer;
	Shader* m_shader;
	ID3D10Device* m_d3dDevice;
};
#endif