#include "Ground.h"

Ground::Ground()
{

}

Ground::~Ground()
{
	m_vertexBuffer->~Buffer();
	m_shader->~Shader();
}

HRESULT Ground::Initialize(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;

	Vertex pData[] = {
		Vertex(D3DXVECTOR3(-100, -50, 100), D3DXVECTOR2(1,0)),
		Vertex(D3DXVECTOR3(-100,-50,-100), D3DXVECTOR2(0,0)),
		Vertex(D3DXVECTOR3(100,-50,100), D3DXVECTOR2(1,1)),
		Vertex(D3DXVECTOR3(100,-50,-100), D3DXVECTOR2(0,1))
	};

	//Create Vertex Buffer Description
	BUFFER_INIT_DESC bdVertex;
	bdVertex.ElementSize = sizeof(Vertex);
	bdVertex.InitData = pData;
	bdVertex.NumElements = 4;
	bdVertex.Type = VERTEX_BUFFER;
	bdVertex.Usage = BUFFER_DEFAULT;
	
	//Create Buffer
	m_vertexBuffer = new Buffer();
	if(FAILED(m_vertexBuffer->Init(m_d3dDevice, bdVertex)))
	{
		return E_FAIL;
	}

	// Define our vertex data layout
	const D3D10_INPUT_ELEMENT_DESC VertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(D3DXVECTOR3), D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Create Shader
	m_shader = new Shader();
	if(FAILED(m_shader->Init(m_d3dDevice, "Ground.fx", VertexLayout, sizeof(VertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "DrawLine",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	//Create texture view
	//ID3D10ShaderResourceView *pSRView = NULL;
	D3DX10CreateShaderResourceViewFromFile( m_d3dDevice, "Ground.png", NULL, NULL, &pSRView, NULL );

	m_shader->SetResource("myTexture", pSRView);
	//pSRView->Release();

	return S_OK;
}

void Ground::PrepareToRender(D3DXMATRIX& p_mMVP, D3DXMATRIX& p_mLMVP, D3DXVECTOR2& pSMAPSize)
{
	// Set Input Assembler params
	m_d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_shader->SetResource("myTexture", pSRView);
	m_shader->SetMatrix("g_mWorldViewProjection", p_mMVP);
	m_shader->SetMatrix("g_mLightWVP",p_mLMVP);
	m_shader->SetFloat("SMAP_SIZE_X", pSMAPSize.x);
	m_shader->SetFloat("SMAP_SIZE_Y", pSMAPSize.y);
	
	//set Vertex buffer
	m_vertexBuffer->Apply(0);
}

void Ground::Render()
{
	// Render line using the technique g_pRenderTextured
	D3D10_TECHNIQUE_DESC techDesc;
	m_shader->GetTechnique()->GetDesc( &techDesc );
	
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		m_shader->Apply(p);
		m_d3dDevice->Draw(4,0);
	}
}

void Ground::RenderSpecificShader(Shader* pShader, D3DXMATRIX& p_mLMVP)
{
	m_d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	pShader->SetTechniqueByName("DrawLine");
	pShader->SetMatrix("g_mLightWVP",p_mLMVP);

	m_vertexBuffer->Apply(0);

	D3D10_TECHNIQUE_DESC techDesc;
	pShader->GetTechnique()->GetDesc( &techDesc );
	
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		pShader->Apply(p);
		m_d3dDevice->Draw(4,0);
	}
}
void Ground::SetShadowMap(ID3D10ShaderResourceView* pTex)
{
	m_shader->SetResource("myShaderTex", pTex);
}

ID3D10ShaderResourceView * Ground::getShaderResource()
{
	return pSRView;
}

void Ground::SetGroundTexNULL()
{
	m_shader->SetResource("myTexture", 0);
	m_shader->SetResource("myShaderTex", 0);
	m_shader->GetTechniqueByName("DrawLine")->GetPassByIndex(0)->Apply(0);
}