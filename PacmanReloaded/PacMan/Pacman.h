#ifndef PACMAN_H
#define PACMAN_H
#include "Object.h"

class Pacman : public Object
{
public:
	Pacman(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position);
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
	//D3DXVECTOR3		getDirection();
private:
	float m_speed, m_speedScale;
	//D3DXVECTOR3	m_direction;
};
#endif