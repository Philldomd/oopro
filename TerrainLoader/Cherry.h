#ifndef CHERRY_H
#define CHERRY_H
#include "Object.h"

class Cherry : public Object
{
public:
	Cherry(ID3D10Device* p_device, D3DXVECTOR3 p_position);
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
private:
	UINT m_score;
	float m_lifeTime;
};
#endif