#include "WallInstancing.h"

WallInstancing::WallInstancing(ID3D10Device* p_device)
{
	m_device = p_device;
	D3DXMatrixIdentity(&m_rotation);
}

WallInstancing::~WallInstancing()
{
	m_objects = NULL;
	m_device = NULL;
	SAFE_RELEASE(m_VB[1]);
	mInstanceData = NULL;
	SAFE_RELEASE(m_VB[0]);
}

void WallInstancing::initialize(vector<Object*>* p_objects)
{
	m_objects = p_objects;

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
	fx::InstanceFX->setResource("g_txDiffuse",m_objects->at(0)->getModel()->m_material->m_textureResource);
	
}

void WallInstancing::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	//If there's objects in vector
	if(!m_objects->empty())
	{
		m_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		fx::InstanceFX->setMatrix("g_mWorldViewProj", p_view * p_projection);
		fx::InstanceFX->setMatrix("g_mRotation",m_rotation);
		
		
		UINT Offsets[2] = {0,0};

		//Always same model as the rest of objects
		m_VB[0] = m_objects->at(0)->getModel()->m_vertexBuffer->getBufferPointer();
		m_VB[1] = mInstanceData->getBufferPointer();

		m_strides[0] = ( UINT )m_objects->at(0)->getModel()->m_vertexBuffer->getVertexSize();
		m_strides[1] = sizeof( D3DXMATRIX );

		m_device->IASetVertexBuffers( 0, 2, m_VB, m_strides, Offsets );
		m_objects->at(0)->getModel()->m_indexBuffer->apply(0);

		D3D10_TECHNIQUE_DESC techDesc;
		fx::InstanceFX->getTechnique()->GetDesc( &techDesc );
	
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{		
			fx::InstanceFX->apply(0);
			m_device->DrawIndexedInstanced( ( UINT )m_objects->at(0)->getModel()->m_size, m_objects->size(),
                                            0, 0, 0 );
		}
	}
}