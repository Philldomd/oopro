#ifndef POWERUP_H
#define POWERUP_H
#include "Object.h"

class PowerUp : public Object
{
public:
	PowerUp(ID3D10Device* p_device, D3DXVECTOR3 p_position);
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
private:
};
#endif