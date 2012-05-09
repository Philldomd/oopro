#include "Wall.h"

Wall::Wall(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size) 
		:Object(p_device, p_position)
{
	m_size = p_size;
	m_model = p__model;
	m_height = ((m_size.x + m_size.y) / 2) * 0.5f;
	D3DXMatrixScaling(&m_scale, m_size.x, m_height, m_size.y);
}
void Wall::initialize()
{

}

void Wall::update(float p_deltaTIme)
{
}