#include "Factory.h"
//####################
//	PacmanFactory
//####################
Pacman* PacmanFactory::createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
{
	return new Pacman(p_device, m_modelManager->getModelByName("Pacman"),p_position, p_size);
}

//####################
//	EnemyFactory
//####################

Object* EnemyFactory::createObjectInstance()
{
	switch(m_type)
	{
		case BLINKY:
			return NULL;
			break;
		case PINKY:
			return NULL;
			break;
		case INKY:
			return NULL;
			break;
		case CLYDE:
			return NULL;
			break;
		default:
			return NULL;
			break;
	}
}
//####################
//	WallFactory
//####################

Object* WallFactory::createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
{
	return new Wall(p_device, m_modelManager->getModelByName("Wall"),p_position, p_size);
}
//####################
//	CandyFactory
//####################
Object* CandyFactory::createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
{
	return new Candy(p_device, m_modelManager->getModelByName("Candy") ,p_position, p_size);
}
//####################
//	PowerUpFactory
//####################
Object* PowerUpFactory::createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
{
	return new PowerUp(p_device, m_modelManager->getModelByName("Candy") ,p_position, p_size);
}
//####################
//	CherryFactory
//####################
Object* CherryFactory::createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size)
{
	return new Cherry(p_device, m_modelManager->getModelByName("Cherry") ,p_position, p_size);
}