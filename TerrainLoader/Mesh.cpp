#include "Mesh.h"

Mesh::Mesh()
{
	m_vertexBuffer = NULL;
	m_shader = NULL;
	m_d3dDevice = NULL;
}

Mesh::~Mesh()
{
	m_vertexBuffer->~Buffer();
	m_shader->~Shader();
}

HRESULT Mesh::Initialize(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;

	m_nrVertices = m_vertexArray.size();

	//Create Vertex Buffer Description
	BUFFER_INIT_DESC bdVertex;
	bdVertex.ElementSize = sizeof(Vertex);
	bdVertex.InitData = m_vertexArray.data();
	bdVertex.NumElements = m_nrVertices;
	bdVertex.Type = VERTEX_BUFFER;
	bdVertex.Usage = BUFFER_DEFAULT;
	
	//Create Buffer
	m_vertexBuffer = new Buffer();
	if(FAILED(m_vertexBuffer->Init(m_d3dDevice, bdVertex)))
	{
		return E_FAIL;
	}

	//Delete vertexArray
	Clear();
	ShrinkToFit();
	

	// Define our vertex data layout
	//const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	//{
	//	{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(D3DXVECTOR3), D3D10_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR2), D3D10_INPUT_PER_VERTEX_DATA, 0} 
	//};

	////Create Shader
	//m_shader = new Shader();
	//if(FAILED(m_shader->Init(m_d3dDevice, "Terrain.fx", lineVertexLayout, sizeof(lineVertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "DrawLine",
	//	D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	//{
	//	return E_FAIL;
	//}

	////Create texture view
	//ID3D10ShaderResourceView *pSRView = NULL;
	//D3DX10CreateShaderResourceViewFromFile( m_d3dDevice, "bthcolor.dds", NULL, NULL, &pSRView, NULL );

	//m_shader->SetResource("myTexture", pSRView);
	//pSRView->Release();

	return S_OK;
}

void Mesh::Clear()
{
	m_vertexArray.clear();
}

void Mesh::ShrinkToFit()
{
	vector<Vertex>(m_vertexArray).swap(m_vertexArray);
}

void Mesh::PrepareToRender(D3DXMATRIX& p_mMVP)
{
	// Set Input Assembler params
	m_d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_shader->SetMatrix("g_mWorldViewProjection", p_mMVP);

	//set Vertex buffer
	m_vertexBuffer->Apply(0);
}

void Mesh::Render()
{
	// Render line using the technique g_pRenderTextured
	D3D10_TECHNIQUE_DESC techDesc;
	m_shader->GetTechnique()->GetDesc( &techDesc );
	
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		m_shader->Apply(p);
		m_d3dDevice->Draw(m_nrVertices,0);
	}
}

void Mesh::AddVertex(D3DXVECTOR3 *p_pos, D3DXVECTOR2 *p_texC, D3DXVECTOR3 *p_norm)
{
	m_vertexArray.push_back(Vertex(p_pos, p_texC, p_norm));
}

void Mesh::SetBuffer()
{
	m_vertexBuffer->Apply(0);
}

int Mesh::getNrVertices()
{
	return m_nrVertices;
}
//void Mesh::SetReserveVertexSize(long p_size)
//{
//	m_vertexArray.reserve(p_size);
//}