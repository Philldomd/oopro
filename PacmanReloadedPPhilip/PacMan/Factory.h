#ifndef FACTORY_H
#define FACTORY_H
#include "ModelManager.h"
#include "Pacman.h"
#include "Wall.h"
#include "Candy.h"
#include "Cherry.h"
#include "PowerUp.h"
#include "Enemy.h"

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
	EnemyFactory(ModelManager* p_modelManager)
	{
		m_modelManager = p_modelManager;
	}
	Enemy* createObjectInstance(ID3D10Device* p_device, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size, Enemy::Type p_type,
											vector<vector<char>>* p_mapMatrix);
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