#ifndef SCENE_
#define SCENE_

#include "d3dUtil.h"
#include <vector>

class Scene
{
public:
	Scene(ID3D10Device* p_d3dDevice);
	virtual ~Scene();
	virtual void  draw(ID3DX10Sprite * p_spriteBatch);
	virtual void  update(float p_deltaTime);

protected:	
	ID3D10Device * m_d3dDevice;

};

#endif