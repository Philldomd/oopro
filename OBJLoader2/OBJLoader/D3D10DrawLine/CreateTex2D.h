#ifndef CREATETEX2D__H
#define CREATETEX2D__H

#include "stdafx.h"

class CreateTex
{
public:
	CreateTex();
	~CreateTex();
	void Initilize(ID3D10Device* p_Device, float p_Height, float p_Width);
	HRESULT CreateShadowTex();
	ID3D10DepthStencilView* getSDepthV();
	ID3D10ShaderResourceView* getSResourceV();
	const D3D10_VIEWPORT* getViewPort();

private:
	int m_width;
	int m_height;
	ID3D10Device* m_Device;
	ID3D10Texture2D* ShadowTex;
	ID3D10DepthStencilView* ShadowDethView;
	ID3D10ShaderResourceView* ShadowTexSRView;
	D3D10_VIEWPORT vp;
};


#endif