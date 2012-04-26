#ifndef DRAWTEX2D_H
#define DRAWTEX2D_H

#include "stdafx.h"
#include "Shader.h"

class DrawTex2D
{
public:
	DrawTex2D();
	~DrawTex2D();

	HRESULT Initialize(ID3D10Device* p_d3dDevice);
	void PrepareToRender(D3DXMATRIX& p_mMVP);
	void Render();
	ID3D10ShaderResourceView* getDepthRs();
	ID3D10DepthStencilView* getRenderTD();

	void toggleSMAPSize();

	UINT getTexSizeX();
	UINT getTexSizeY();

	const D3D10_VIEWPORT* getViewport();

private:
	HRESULT CreateShadowMap();

	ID3D10Device* m_d3dDevice;
	ID3D10Texture2D* depthBuffer;
	ID3D10ShaderResourceView* m_depthResource;
	ID3D10RenderTargetView* m_renderthatshit;
	ID3D10DepthStencilView* m_renderTargetDepth;

	D3D10_VIEWPORT m_Viewport;
	UINT m_width;
	UINT m_height;
};
#endif