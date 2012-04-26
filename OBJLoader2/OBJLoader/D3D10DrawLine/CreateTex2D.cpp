#include "CreateTex2D.h"

CreateTex::CreateTex()
{
	ShadowDethView = NULL;
	ShadowTexSRView = NULL;
	m_height = 1024;
	m_width = 1024;
}
CreateTex::~CreateTex()
{

}
void CreateTex::Initilize(ID3D10Device* p_Device, float p_HeightMultiplay, float p_WidthMultiplay)
{
	m_Device = p_Device;
	m_height = m_height * p_HeightMultiplay;
	m_width = m_width * p_WidthMultiplay;

	
	vp.Height = m_height;
	vp.Width = m_width;
	vp.TopLeftY = 0;
	vp.TopLeftX = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;


}
HRESULT CreateTex::CreateShadowTex()
{
	//Create Shadow map Texture desc
	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	if( FAILED( m_Device->CreateTexture2D( &texDesc, NULL, &ShadowTex ))) return E_FAIL;

	//Create the depth stencil view desc
	D3D10_DEPTH_STENCIL_VIEW_DESC descShadow;
	descShadow.Format = DXGI_FORMAT_D32_FLOAT;
	descShadow.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descShadow.Texture2D.MipSlice = 0;

	if( FAILED( m_Device->CreateDepthStencilView( ShadowTex, &descShadow, &ShadowDethView))) 
	{
		return E_FAIL;
	}
	//Create shader resource view desc
	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	//create texture and deth/resource views

	
	if( FAILED( m_Device->CreateShaderResourceView( ShadowTex, &srvDesc, &ShadowTexSRView))) return E_FAIL;
	SAFE_RELEASE(ShadowTex);

	return S_OK;
}

ID3D10DepthStencilView* CreateTex::getSDepthV()
{
	return ShadowDethView;
}

ID3D10ShaderResourceView* CreateTex::getSResourceV()
{
	return ShadowTexSRView;
}

const D3D10_VIEWPORT* CreateTex::getViewPort()
{
	return &vp;
}