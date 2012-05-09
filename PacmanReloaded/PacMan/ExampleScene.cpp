#include "ExampleScene.h"

ExampleScene::ExampleScene() : Scene(){}

void ExampleScene::init( ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort )
{
	//SKAPA EN VANLIG SPRITE
	m_waddaSprite = new Sprite(p_d3dDevice, 0, 0, 1, 1, "bg.jpg", p_viewPort);
	//SÄTT SPRITEN TILL FULLSCREEN
	m_waddaSprite->setAnchorPoint(Sprite::FULLSCREEN);

	//SKAPA EN KNAPP
	m_waddaButton = new SpriteButton(p_d3dDevice, 0, 0, 1, 1, "bg.jpg", p_viewPort);
	//SÄTT SPRITEN TILL FULLSCREEN
	m_waddaButton->setAnchorPoint(Sprite::FULLSCREEN);
}

void ExampleScene::draw( ID3DX10Sprite * p_spriteBatch )
{
	//RITA SPRITE
	//m_waddaSprite->draw(p_spriteBatch);

	//RITA KNAPP
	m_waddaButton->draw(p_spriteBatch);
}

void ExampleScene::update( float p_deltaTime )
{
}


ExampleScene::~ExampleScene()
{

}


