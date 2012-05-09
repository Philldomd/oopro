#include "Instancing.h"

Instancing::Instancing(ID3D10Device* p_device)
{
	m_device = p_device;
	D3DXMatrixIdentity(&m_rotation);
	m_shader = NULL;
	m_angle = 0.0f;
}

Instancing::~Instancing()
{
	m_shader = NULL;
	
	m_device = NULL;
	mInstanceData = NULL;
	m_VB[0] = NULL;
	m_VB[1] = NULL;

	m_objects = NULL;
}

void Instancing::initializeDefault(vector<Object*>* p_objects, Shader* p_shader)
{
	m_objects = p_objects;
	m_shader = p_shader;

	//Init data
	D3DXMATRIX* initMatrices = new D3DXMATRIX[m_objects->size()];

	for(UINT i = 0; i < m_objects->size(); i++)
	{
		initMatrices[i] = m_objects->at(i)->getWorldMatrix();
	}

	BUFFER_INIT_DESC bdInstance;
	//Instance Buffer Description
	bdInstance.ElementSize = sizeof( D3DXMATRIX );
	bdInstance.InitData = initMatrices;
	bdInstance.NumElements = m_objects->size();
	bdInstance.Type = VERTEX_BUFFER;
	bdInstance.Usage = BUFFER_DEFAULT;

	mInstanceData = new Buffer();
	mInstanceData->init(m_device, bdInstance);

	//Delete init data
	SAFE_DELETE_ARRAY(initMatrices);

}

void Instancing::initializeDynamic(vector<Object*>* p_objects, Shader* p_shader)
{
	m_objects = p_objects;

	m_shader = p_shader;

	m_previousObjectSize = m_objects->size();
	//Init data
	D3DXMATRIX* initMatrices = new D3DXMATRIX[m_objects->size()];

	for(UINT i = 0; i < m_objects->size(); i++)
	{
		initMatrices[i] = m_objects->at(i)->getWorldMatrix();
	}

	BUFFER_INIT_DESC bdInstance;
	//Instance Buffer Description
	bdInstance.ElementSize = sizeof( D3DXMATRIX );
	bdInstance.InitData = initMatrices;
	bdInstance.NumElements = m_objects->size();
	bdInstance.Type = VERTEX_BUFFER;
	bdInstance.Usage = BUFFER_CPU_WRITE_DISCARD;

	mInstanceData = new Buffer();
	mInstanceData->init(m_device, bdInstance);

	//Delete init data
	SAFE_DELETE_ARRAY(initMatrices);
}

void Instancing::updateDefault(float p_gameTime)
{

}
void Instancing::updateDynamic(float p_gameTime)
{
	m_angle += p_gameTime;
	D3DXMatrixRotationY(&m_rotation, m_angle);

	//Update buffer if number of candies has changed
	if(m_previousObjectSize != m_objects->size())
	{
		D3DXMATRIX* initMatrices = new D3DXMATRIX[m_objects->size()];
		for(UINT i = 0; i < m_objects->size(); i++)
		{
			initMatrices[i] = m_objects->at(i)->getWorldMatrix();
		}

		D3DXMATRIX* pMatrices = NULL;

		mInstanceData->map(( void** )&pMatrices);

		memcpy( pMatrices, initMatrices, m_objects->size() * sizeof( D3DXMATRIX ) );

		mInstanceData->unMap();

		SAFE_DELETE_ARRAY(initMatrices);

		m_previousObjectSize = m_objects->size();
	}
}

void Instancing::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	//If there's objects in vector
	if(!m_objects->empty())
	{
		m_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_shader->setMatrix("g_mWorldViewProj", p_view * p_projection);
		m_shader->setMatrix("g_mRotation",m_rotation);

		//if(m_objects->at(0)->getModel()->m_material->m_textureResource)
			//m_shader->setResource("g_txDiffuse",m_objects->at(0)->getModel()->m_material->m_textureResource);
		
		UINT Offsets[2] = {0,0};

		//Always same model as the rest of objects
		m_VB[0] = m_objects->at(0)->getModel()->m_vertexBuffer->getBufferPointer();
		m_VB[1] = mInstanceData->getBufferPointer();

		m_strides[0] = ( UINT )m_objects->at(0)->getModel()->m_vertexBuffer->getVertexSize();
		m_strides[1] = sizeof( D3DXMATRIX );

		m_device->IASetVertexBuffers( 0, 2, m_VB, m_strides, Offsets );
		m_objects->at(0)->getModel()->m_indexBuffer->apply(0);

		D3D10_TECHNIQUE_DESC techDesc;
		m_shader->getTechnique()->GetDesc( &techDesc );
	
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{		
			m_shader->apply(p);
			m_device->DrawIndexedInstanced( ( UINT )m_objects->at(0)->getModel()->m_size, m_objects->size(),
                                            0, 0, 0 );
		}
	}
}

void Instancing::temp(vector<Object*>* p_objects)
{
	m_shader->setResource("g_txDiffuse",p_objects->at(0)->getModel()->m_material->m_textureResource);
}