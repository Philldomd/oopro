#include "Minimap.h"

MiniMap::MiniMap()
{
	m_miniRTV = NULL;
	m_miniTexSRV = NULL;
	m_height = 512;
	m_width = 512;
	m_miniMapShader = new Shader();
}

MiniMap::~MiniMap()
{
	SAFE_RELEASE(m_miniRTV);
	SAFE_RELEASE(m_miniTexSRV);
	SAFE_RELEASE(m_miniMap);
}

void MiniMap::initialize(ID3D10Device* p_Device)
{
	m_Device = p_Device;
	
	vp.Height = m_height;
	vp.Width = m_width;
	vp.TopLeftY = 0;
	vp.TopLeftX = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	D3DXMatrixLookAtLH(&m_View, &D3DXVECTOR3(0,500,0),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(-1, 0, 0));
	D3DXMatrixOrthoLH(&m_ViewVolume, 200, 200, 1.0f, 1024.0f);

	m_ViewWVP = m_View * m_ViewVolume;

	const D3D10_INPUT_ELEMENT_DESC minimapLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	m_miniMapShader->init(m_Device, 
		"miniMap.fx", 
		minimapLayout, 
		sizeof(minimapLayout)/ sizeof(D3D10_INPUT_ELEMENT_DESC),
		"RB", 
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY);
	
}

HRESULT MiniMap::CreateTex()
{
	//Create texture desc
	D3D10_TEXTURE2D_DESC texDesc;
	ZeroMemory( &texDesc, sizeof(texDesc) );
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	if(FAILED( m_Device->CreateTexture2D( &texDesc, NULL, &m_miniMap)))
	{
		return E_FAIL;
	}
	////Create the render target view desc
	D3D10_RENDER_TARGET_VIEW_DESC descMap;
	descMap.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	descMap.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descMap.Texture2D.MipSlice = 0;

	if(FAILED( m_Device->CreateRenderTargetView( m_miniMap, &descMap, &m_miniRTV)))
		return E_FAIL;

	//Create shader resource view desc
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if(FAILED( m_Device->CreateShaderResourceView( m_miniMap, &srvDesc, &m_miniTexSRV))) 
		return E_FAIL;
	//SAFE_RELEASE(m_minimap);

	return S_OK;
}

void MiniMap::prepareRender(ID3D10DepthStencilView* p_depthView)
{
	//m_Device->OMSetRenderTargets(1,&m_miniRTV,p_depthView);
	m_Device->RSSetViewports(1,&vp);
	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_Device->ClearRenderTargetView(m_miniRTV, ClearColor);
	m_Device->ClearDepthStencilView(p_depthView, D3D10_CLEAR_DEPTH, 1.0f, 0);
	m_miniMapShader->setTechniqueByName("RB");
}

void MiniMap::normalRender()
{
	m_Device->OMSetRenderTargets(0,0,NULL);
}

void MiniMap::finalRender()
{
	m_Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_miniMapShader->setResource("miniMap", m_miniTexSRV);
	m_miniMapShader->setTechniqueByName("RB");
	m_miniMapShader->apply(0);

	m_Device->Draw(1,0);

	//Release minimap
	m_miniMapShader->setResource("miniMap", 0);
	m_miniMapShader->getTechniqueByName("RB")->GetPassByIndex(0)->Apply(0);
}

ID3D10RenderTargetView* MiniMap::getRTV()
{
	return m_miniRTV;
}

ID3D10ShaderResourceView* MiniMap::getSRV()
{
	return m_miniTexSRV;
}

const D3D10_VIEWPORT* MiniMap::getVP()
{
	return &vp;
}

D3DXMATRIX MiniMap::getWVPMatrix()
{
	return m_ViewWVP;
}