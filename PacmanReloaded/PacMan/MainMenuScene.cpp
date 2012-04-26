#include "MainMenuScene.h"

MainMenuScene::MainMenuScene( ID3D10Device* p_d3dDevice ) : Scene( p_d3dDevice)
{
	ModelManager* modelManager = new ModelManager(p_d3dDevice);
	modelManager->createModel("modelOne", "ghost.obj");
	Model* temp = modelManager->getModelByName("modelOne");
}

void MainMenuScene::draw( ID3DX10Sprite * p_spriteBatch )
{

}

void MainMenuScene::update( float p_deltaTime )
{

}

MainMenuScene::~MainMenuScene()
{

}
