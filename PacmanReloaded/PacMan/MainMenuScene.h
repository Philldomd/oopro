#ifndef MENUSCENE
#define MENUSCENE

#include "d3dUtil.h"
#include "Scene.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene(ID3D10Device* p_d3dDevice);
	~MainMenuScene();

	void  draw(ID3DX10Sprite * p_spriteBatch);
	void  update(float p_deltaTime);

private:

};

#endif