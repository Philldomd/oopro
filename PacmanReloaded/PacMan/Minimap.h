#ifndef MINIMAP_H
#define MINIMAP_H

#include "stdafx.h"
#include "Shader.h"
#include "d3dApp.h"

class MiniMap
{
public:
	MiniMap();
	~MiniMap();

	void initialize(ID3D10Device* p_d3dDevice);
	HRESULT CreateTex();
	void prepareRender(ID3D10DepthStencilView* p_depthView, D3D10_VIEWPORT p_VP);
	void normalRender(ID3D10DepthStencilView* p_depthView,ID3D10RenderTargetView* p_renderTarget,D3D10_VIEWPORT p_VP);
	void finalRender();
	
	ID3D10RenderTargetView* getRTV();
	ID3D10ShaderResourceView* getSRV();
	const D3D10_VIEWPORT* getVP();
	D3DXMATRIX getViewMatrix();
	D3DXMATRIX getProjectionMatrix();

private:
	int m_width;
	int m_height;
	
	Shader* m_miniMapShader;
	D3DXMATRIX m_view, m_viewVolume;
	ID3D10Device* m_Device;
	ID3D10Texture2D* m_miniMap;
	ID3D10RenderTargetView* m_miniRTV;
	ID3D10ShaderResourceView* m_miniTexSRV;
	D3D10_VIEWPORT vp;
};


#endif