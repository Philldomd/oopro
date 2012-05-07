#ifndef WALL_H
#define WALL_H
#include "Object.h"

class Wall : public Object
{
public:
	Wall(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
private:
	D3DXVECTOR2 m_size;
	float		m_height;
};
#endif