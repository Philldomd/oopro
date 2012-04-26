#include "CandyInstancing.h"

CandyInstancing::CandyInstancing(ID3D10Device* p_device)
{
	m_device = p_device;

	m_angle = 0.0f;

	D3DXMatrixRotationY(&m_rotation, m_angle);
}

CandyInstancing::~CandyInstancing()
{
	m_objects = NULL;
	m_device = NULL;
	mInstanceData->~Buffer();
	mInstanceData = NULL;
	SAFE_RELEASE(m_VB[0]);
	SAFE_RELEASE(m_VB[1]);
}

void CandyInstancing::initialize(vector<Object*>* p_objects)
{
	m_objects = p_objects;

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

void CandyInstancing::update(float p_gameTime)
{
	m_angle += p_gameTime;

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

void CandyInstancing::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	//If there's objects in vector
	if(!m_objects->empty())
	{
		D3DXMatrixRotationY(&m_rotation, m_angle);
		fx::InstanceFX->setMatrix("g_mRotation",m_rotation);
		fx::InstanceFX->setMatrix("g_mWorldViewProj", p_view * p_projection);

		UINT Offsets[2] = {0,0};

		//Always same model as the rest of objects
		//pVB[0] = m_objects->at(0)->getModel()->getVertexBuffer()->getBufferpointer();
		m_VB[1] = mInstanceData->getBufferPointer();

		//Strides[0] = ( UINT )m_objects->at(0)->getModel()->getVertexBuffer()->getBufferpointer()->GetVertexSize();
		m_strides[1] = sizeof( D3DXMATRIX );

		m_device->IASetVertexBuffers( 0, 2, m_VB, m_strides, Offsets );
		//m_objects->at(0)->getModel()->getIndexBuffer()->apply(0)

		D3D10_TECHNIQUE_DESC techDesc;
		fx::InstanceFX->getTechnique()->GetDesc( &techDesc );

		for( UINT p = 0; p < techDesc.Passes; ++p )
		{		
			fx::InstanceFX->apply(0);
			//m_device->DrawIndexedInstanced( ( UINT )m_objects->at(0)->getModel()->getIndexBuffer()->getIndexCount(), m_objects->size(),
			//                                0, 0, 0 );
		}
	}
}