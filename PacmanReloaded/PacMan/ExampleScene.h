#ifndef EXAMPLESCENE_
#define EXAMPLESCENE_

#include "d3dUtil.h"
#include "Scene.h"
#include "ModelManager.h"
#include "Sprite.h"
#include "SpriteButton.h"

class ExampleScene : public Scene
{
public:
	ExampleScene();
	~ExampleScene();

	void  init(ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort);
	void  draw(ID3DX10Sprite * p_spriteBatch);
	void  update(float p_deltaTime);

private:

	Sprite* m_waddaSprite;
	SpriteButton* m_waddaButton;
	D3D10_VIEWPORT * m_viewPort;

};

#endif