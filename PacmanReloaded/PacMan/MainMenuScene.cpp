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
	m_worldLoader->loadFromFile("MapTest.png", 512, 512, 5, m_objects);

	//Instancing
	m_wallInstancing = new WallInstancing(p_d3dDevice);
	m_wallInstancing->initialize(&m_objects.m_walls);
	m_candyInstancing = new CandyInstancing(p_d3dDevice);
	m_candyInstancing->initialize(&m_objects.m_candies);

	//TEMPORARY STUFF REMOVE !!
	m_camera = new Camera();
	float aspect = float(800)/600;	
	m_camera->perspective(45, aspect, 0.5f, 2048.0f);
	m_camera->lookAt(D3DXVECTOR3(0,25,-60),m_objects.m_walls.at(200)->getPosition(),D3DXVECTOR3(0,1,0));
	
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


	float setY,walk;
	setY = 0;
	walk = 0;

	//TEMP STUFF
	if(GetAsyncKeyState('A') & 0x8000)
	{
		m_camera->rotate(10, 0);
	}
	if(GetAsyncKeyState('D') & 0x8000)
	{
		m_camera->rotate((-10), 0);
	}
	if(GetAsyncKeyState('W') & 0x8000)
	{
		walk = 1;
	}
	if(GetAsyncKeyState('S') & 0x8000)
	{
		m_camera->rotate(0,0);
		m_camera->setTarget(m_camera->getTarget());
		walk = -1;
	}

	if(GetAsyncKeyState('R') & 0x8000)
	{
		setY = 0.01f;
	}
	else if(GetAsyncKeyState('F') & 0x8000)
	{
		setY = -0.01f;
	}
	else
	{
		//m_camera->rotate(0,0);
	}
	D3DXVECTOR3 lol = m_camera->getTarget();
	m_camera->setTarget(D3DXVECTOR3(lol.x + walk, lol.y + setY, lol.z));
	m_camera->update(p_deltaTime);
}

MainMenuScene::~MainMenuScene()
{

}
