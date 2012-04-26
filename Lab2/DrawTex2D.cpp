#include "DrawTex2D.h"

DrawTex2D::DrawTex2D()
{
	m_depthResource = NULL;
	m_renderTargetDepth = NULL;
	m_width = 640;
	m_height = 480;
}
DrawTex2D::~DrawTex2D()
{
	SAFE_RELEASE(m_renderTargetDepth);
	SAFE_RELEASE(m_depthResource);
	SAFE_RELEASE(depthBuffer);
}
HRESULT DrawTex2D::Initialize(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;
	CreateShadowMap();


	//Create ViewPorts
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = m_width;
	m_Viewport.Height = m_height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	return S_OK;
}

HRESULT DrawTex2D::CreateShadowMap()
{
	//Create Texture description
	D3D10_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = m_width;
	depthDesc.Height = m_height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D10_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	//Create depth Texture
	if(FAILED(m_d3dDevice->CreateTexture2D( &depthDesc, NULL, &depthBuffer)))
	{
		return E_FAIL;
	}
	D3D10_BUFFER_RTV haj;
	haj.NumElements =1;
	haj.FirstElement =0;
	D3D10_RENDER_TARGET_VIEW_DESC renderDesc;
	renderDesc.Buffer = haj;
	renderDesc.Format = DXGI_FORMAT_D32_FLOAT;
	renderDesc.Texture2D.MipSlice = 0;
	
	if(FAILED(m_d3dDevice->CreateRenderTargetView(NULL,&renderDesc,&m_renderthatshit)))
	{
		return E_FAIL;
	}

	//Create depth-stencil description
	D3D10_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	if(FAILED(m_d3dDevice->CreateDepthStencilView( depthBuffer, &dsvDesc, &m_renderTargetDepth)))
	{
		return E_FAIL;
	}

	//Create shader resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = depthDesc.MipLevels;

	if(FAILED(m_d3dDevice->CreateShaderResourceView( depthBuffer, &srDesc, &m_depthResource)))
	{
		return E_FAIL;
	}

	SAFE_RELEASE(depthBuffer);

	return S_OK;
}

void DrawTex2D::toggleSMAPSize()
{
	if(m_width == 640)
	{
		m_width *= 2;
		m_height *= 2;
	}
	else
	{
		m_width /= 2;
		m_height /= 2;
	}
	
	SAFE_RELEASE(m_renderTargetDepth);
	SAFE_RELEASE(m_depthResource);
	SAFE_RELEASE(depthBuffer);

	m_Viewport.Width = m_width;
	m_Viewport.Height = m_height;

	CreateShadowMap();
}

ID3D10ShaderResourceView* DrawTex2D::getDepthRs()
{
	return m_depthResource;
}

ID3D10DepthStencilView* DrawTex2D::getRenderTD()
{
	return m_renderTargetDepth;
}

const D3D10_VIEWPORT* DrawTex2D::getViewport()
{
	return &m_Viewport;
}

UINT DrawTex2D::getTexSizeX()
{
	return m_width;
}
UINT DrawTex2D::getTexSizeY()
{
	return m_height;
}