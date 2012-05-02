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

Object* EnemyFactory::createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size, Enemy::Type p_type,
											vector<vector<char>> p_mapMatrix)
{
	switch(p_type)
	{
		case Enemy::BLINKY:
			return new Enemy(p_device, m_modelManager->getModelByName("Candy"),p_position, p_size, p_mapMatrix, p_type);
			break;
		case Enemy::PINKY:
			return new Enemy(p_device, m_modelManager->getModelByName("Candy"),p_position, p_size, p_mapMatrix, p_type);
			break;
		case Enemy::INKY:
			return new Enemy(p_device, m_modelManager->getModelByName("Candy"),p_position, p_size, p_mapMatrix, p_type);
			break;
		case Enemy::CLYDE:
			return new Enemy(p_device, m_modelManager->getModelByName("Candy"),p_position, p_size, p_mapMatrix, p_type);
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