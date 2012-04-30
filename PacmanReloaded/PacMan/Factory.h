#ifndef FACTORY_H
#define FACTORY_H
#include "ModelManager.h"

#include "Wall.h"
#include "Candy.h"

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
	Object* createObjectInstance();
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
	Object* createObjectInstance();
};

class CherryFactory : public Factory
{
public:
	Object* createObjectInstance();
};
#endif