#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
#include "AI.h"
#include "BlinkyAI.h"
#include "InkyAI.h"
#include "PinkyAI.h"
#include "ClydeAI.h"

class Enemy : public Object
{
public:
	enum Type
	{
		BLINKY,
		PINKY,
		INKY,
		CLYDE
	};
	Enemy(	ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, 
		D3DXVECTOR2 p_size,vector<vector<char>>* p_mapMatrix, Type p_type);
	virtual ~Enemy();
	virtual void	initialize();
	virtual void	update(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection, float gameTime); //Speed depends on return enum, chase, shatter, frightened
private:
	AI* m_ai;
	float m_speed, m_speedScale;
	D3DXVECTOR2 m_direction;
	D3DXVECTOR2 m_tileSize; //Pixelsize
	vector<vector<char>>* m_mapMatrix;
	Type m_type;

};
#endif