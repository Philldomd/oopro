#ifndef CANDY_H
#define CANDY_H
#include "Object.h"

class Candy : public Object
{
public:
	Candy(ID3D10Device* p_device, D3DXVECTOR3 p_position);
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
private:
	UINT m_score;
};
#endif