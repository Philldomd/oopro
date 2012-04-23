#include "Factory.h"
//####################
//	PacmanFactory
//####################
Object* PacmanFactory::createObjectInstance()
{
	return NULL;
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
	return new Wall(p_device, p_position, p_size);
}
//####################
//	CandyFactory
//####################
Object* CandyFactory::createObjectInstance()
{
	return NULL;
}
//####################
//	PowerUpFactory
//####################
Object* PowerUpFactory::createObjectInstance()
{
	return NULL;
}
//####################
//	CherryFactory
//####################
Object* CherryFactory::createObjectInstance()
{
	return NULL;
}