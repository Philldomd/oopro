#include "Pacman.h"

Pacman::Pacman(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
	:Object(p_device, p_position)
{
	//m_size = p_size;
	m_model = p__model;

	m_forwardSpeed = 20.0f;
	m_headingSpeed = 120.0f;
	m_velocity = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_eulerOrient = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//TEMP
	//initialize();
}

void Pacman::initialize()
{
	createBoundingVolume();
	m_volume->updatePosition( m_scale, m_translate );
}

void Pacman::update(float p_deltaTime)
{
	m_position += m_velocity * m_forwardSpeed * p_deltaTime;

	//m_volume->updatePosition( m_scale, m_translate );
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