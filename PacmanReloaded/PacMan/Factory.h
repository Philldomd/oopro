#ifndef FACTORY_H
#define FACTORY_H
#include "ModelManager.h"
#include "Pacman.h"
#include "Wall.h"
#include "Candy.h"
#include "Cherry.h"
#include "PowerUp.h"

class Factory
{
public:
	virtual Object* createObjectInstance()  { return NULL; };
protected:
	ModelManager* m_modelManager;
};

class PacmanFactory : public Factory
{
public:
	PacmanFactory(ModelManager* p_modelManager)
	{
		m_modelManager = p_modelManager;
	}
	Pacman* createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
};

class EnemyFactory : public Factory
{
public:
	enum Type
	{
		BLINKY,
		PINKY,
		INKY,
		CLYDE
	};
	Object* createObjectInstance();
private:
	Type m_type;
};

class WallFactory : public Factory
{
public:
	WallFactory(ModelManager* p_modelManager)
	{
		m_modelManager = p_modelManager;
	}
	Object* createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
};

class CandyFactory : public Factory
{
public:
	CandyFactory(ModelManager* p_modelManager)
	{
		m_modelManager = p_modelManager;
	}
	Object* createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
};

class PowerUpFactory : public Factory
{
public:
	PowerUpFactory(ModelManager* p_modelManager)
	{
		m_modelManager = p_modelManager;
	}
	Object* createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
};

class CherryFactory : public Factory
{
public:
	CherryFactory(ModelManager* p_modelManager)
	{
		m_modelManager = p_modelManager;
	}
	Object* createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
};
#endif