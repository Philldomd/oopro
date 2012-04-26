#include "Candy.h"

Candy::Candy(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position)
	:Object(p_device, p_position)
{
	m_model = p__model;
}

void Candy::initialize()
{

}

void Candy::update(float gameTime)
{

}