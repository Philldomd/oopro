#include "Enemy.h"

Enemy::Enemy(	ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, 
				D3DXVECTOR2 p_size,vector<vector<char>> p_mapMatrix, Type p_type)
	:Object(p_device, p_position)
{
	m_mapMatrix = p_mapMatrix;
	m_model = p__model;
	m_tileSize = p_size;
	float scaleMultiplier = 0.03f;
	D3DXMatrixScaling(&m_scale, scaleMultiplier * p_size.x, 
						scaleMultiplier * (p_size.y+p_size.x)/2, scaleMultiplier * p_size.y);
	m_type = p_type;
}

void Enemy::initialize()
{

}

void Enemy::update(float gameTime)
{

}