#include "Pacman.h"

Pacman::Pacman(ID3D10Device* p_device, Model* p__model1, Model* p__model2, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
	:Object(p_device, p_position)
{
	//m_size = p_size;
	m_model = p__model1;
	m_model2 = p__model2; //Target model

	m_forwardSpeed = 20.0f;
	m_headingSpeed = 120.0f;
	m_velocity = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_eulerOrient = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Lerp
	m_lerpValue = 0.0f;
	m_invertLerp = false;
	m_animationSpeed = 2.0f;

	m_VB[0] = m_model->m_vertexBuffer->getBufferPointer();
	m_VB[1] = m_model2->m_vertexBuffer->getBufferPointer();

	m_IB[0] = m_model->m_indexBuffer->getBufferPointer();
	m_IB[1] = m_model2->m_indexBuffer->getBufferPointer();

	m_strides[0] = ( UINT )m_model->m_vertexBuffer->getVertexSize();
	m_strides[1] = ( UINT )m_model2->m_vertexBuffer->getVertexSize();
}
Pacman::~Pacman()
{
	m_model = NULL;
	m_device = NULL;
}

void Pacman::initialize(Shader* p_shader)
{
	createBoundingVolume();
	m_volume->updatePosition( m_scale, m_translate );

	m_shader = p_shader;
}

void Pacman::update(float p_deltaTime)
{
	//m_position += m_velocity * m_forwardSpeed * p_deltaTime;

	D3DXMatrixTranslation(&m_translate, m_position.x, m_position.y, m_position.z);

	D3DXMatrixLookAtLH(&m_rotation, &m_position, &(m_position + m_velocity), &D3DXVECTOR3(0,1,0));
	m_rotation._41 = 0;
	m_rotation._42 = 0;
	m_rotation._43 = 0;
	D3DXMatrixTranspose(&m_rotation, &m_rotation);

	//Lerp
	if(!m_invertLerp)
	{
		m_lerpValue += m_animationSpeed * p_deltaTime;
		if(m_lerpValue > 1.2f)
			m_invertLerp = true;
	}
	else
	{
		m_lerpValue -= m_animationSpeed * p_deltaTime;
		if(m_lerpValue < 0.0f)
			m_invertLerp = false;
	}

	m_volume->updatePosition( m_scale, m_translate );
}
void Pacman::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	m_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMATRIX worldViewProj = getWorldMatrix() * p_view * p_projection;

	//Manual rotate stuff
	D3DXMatrixRotationY(&m_rotation, -PI);

	UINT Offsets[2] = {0,0};

	m_device->IASetVertexBuffers( 0, 2, m_VB, m_strides, Offsets );
	//m_model->m_vertexBuffer->apply(0);
	m_model->m_indexBuffer->apply(0);
	
	m_shader->setMatrix("g_mWorldViewProj", worldViewProj);	
	m_shader->setFloat("g_mLerpTime", m_lerpValue);

	D3D10_TECHNIQUE_DESC techDesc;
	m_shader->getTechnique()->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		m_shader->apply(p);
		m_device->DrawIndexed( m_model->m_size,0, 0);
	}
}

void Pacman::setDirection(D3DXVECTOR3 p_direction)
{
	m_velocity = p_direction;
}
void Pacman::turnLeft()
{
	D3DXVECTOR4 temp;
	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, -PI/2);
	D3DXVec3Transform(&temp, &m_velocity, &rotation);

	m_velocity.x = temp.x;
	m_velocity.y = temp.y;
	m_velocity.z = temp.z;
}

void Pacman::turnRight()
{
	D3DXVECTOR4 temp;
	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, PI/2);
	D3DXVec3Transform(&temp, &m_velocity, &rotation);

	m_velocity.x = temp.x;
	m_velocity.y = temp.y;
	m_velocity.z = temp.z;
}

void Pacman::uTurn()
{
	m_velocity *= -1.0f;
}

D3DXVECTOR3 Pacman::getDirection()
{
	return m_velocity;
}

void Pacman::walkF(float p)
{
	m_position += m_velocity * m_forwardSpeed * p;
}