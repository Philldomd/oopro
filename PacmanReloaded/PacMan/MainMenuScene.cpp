#include "MainMenuScene.h"

//###########################
//			LEVEL SCENE
//###########################
MainMenuScene::MainMenuScene( ID3D10Device* p_d3dDevice ) : Scene( p_d3dDevice)
{
	m_modelManager = new ModelManager(p_d3dDevice);
	m_modelManager->createModel("Wall", "Box.obj");
	Model* temp = m_modelManager->getModelByName("Wall");

	//WorldLoader
	m_worldLoader = new WorldLoader(p_d3dDevice, m_modelManager);
	m_worldLoader->loadFromFile("MapTestBIG.png", 512, 512, 5, m_objects);

	//Instancing
	m_wallInstancing = new WallInstancing(p_d3dDevice);
	m_wallInstancing->initialize(&m_objects.m_walls);
	m_candyInstancing = new CandyInstancing(p_d3dDevice);
	m_candyInstancing->initialize(&m_objects.m_candies);

	//TEMPORARY STUFF REMOVE !!
	m_camera = new Camera(D3DXVECTOR3( 200, 14, 200 ));
	float aspect = float(800)/600;
	m_camera->createProjectionMatrix(0.25f*PI, aspect, 0.5f, 1000.0f);
	
	fx::InitAll(p_d3dDevice);
}

void MainMenuScene::draw( ID3DX10Sprite * p_spriteBatch )
{
	m_wallInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_candyInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
}

void MainMenuScene::update( float p_deltaTime )
{
	m_candyInstancing->update(p_deltaTime);





	//TEMP STUFF
	if(GetAsyncKeyState('A') & 0x8000)
	{
		m_camera->strafe( -p_deltaTime );
	}
	if(GetAsyncKeyState('D') & 0x8000)
	{
		m_camera->strafe( p_deltaTime );
	}
	if(GetAsyncKeyState('W') & 0x8000)
	{
		m_camera->walk( p_deltaTime );
	}
	if(GetAsyncKeyState('S') & 0x8000)
	{
		m_camera->walk( -p_deltaTime );
	}

	if(GetAsyncKeyState('R') & 0x8000)
	{
		m_camera->setY( p_deltaTime * 100 );
	}
	else if(GetAsyncKeyState('F') & 0x8000)
	{
		m_camera->setY( -p_deltaTime * 100 );
	}
	else{}

	m_camera->updateCameraPos();
	m_camera->updateViewMatrix();
}

MainMenuScene::~MainMenuScene()
{

}
