#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(ID3D10Device* p_device, D3DXVECTOR3 p_position);
	virtual void	initialize();
	virtual void	update(float p_deltaTime); //Speed depends on return enum, chase, shatter, frightened
private:
	//AI m_ai;
	float m_speed, m_speedScale;
	D3DXVECTOR2 m_direction;
	D3DXVECTOR2 m_tileSize; //Pixelsize

};
#endif