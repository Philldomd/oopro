#include "Enemy.h"

Enemy::Enemy(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position)
	:Object(p_device, p_position)
{
}

void Enemy::initialize()
{
	createBoundingVolume();
	m_volume->updatePosition( m_scale, m_translate );
}

void Enemy::update(float gameTime)
{
	m_volume->updatePosition( m_scale, m_translate );
}