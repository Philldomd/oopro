#ifndef MINIMAP_H
#define MINIMAP_H

#include "stdafx.h"
#include "Shader.h"

class Minimap
{
public:
	Minimap();
	~Minimap();

	void initialize(ID3D10Device* p_d3dDevice);
	HRESULT CreateTex();
	ID3D10DepthStencilView* getSDepthV();
	ID3D10ShaderResourceView* getSResourceV();
	const D3D10_VIEWPORT* getViewPort();
	
private:
	int m_width;
	int m_height;
	ID3D10Device* m_Device;
	ID3D10Texture2D* m_minimap;
	ID3D10DepthStencilView* m_miniDV;
	ID3D10ShaderResourceView* m_miniTexSRV;
	D3D10_VIEWPORT vp;
};


#endif