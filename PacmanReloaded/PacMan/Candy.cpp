#include "Candy.h"

Candy::Candy(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
	:Object(p_device, p_position)
{
	m_model = p__model;
	float scaleMultiplier = 0.03f;
	D3DXMatrixScaling(&m_scale, scaleMultiplier * p_size.x, 
						scaleMultiplier * (p_size.y+p_size.x)/2, scaleMultiplier * p_size.y);
}

void Candy::initialize()
{
	createBoundingVolume();
	m_volume->updatePosition( m_scale, m_translate );
}

void Candy::update(float gameTime)
{

}