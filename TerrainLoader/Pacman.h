#ifndef PACMAN_H
#define PACMAN_H
#include "Object.h"

class Pacman : public Object
{
public:
	Pacman(ID3D10Device* p_device, D3DXVECTOR3 p_position);
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
private:
	float m_speed, m_speedScale;
};
#endif