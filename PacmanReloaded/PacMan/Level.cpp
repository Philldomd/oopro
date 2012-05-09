#include "Level.h"

//###########################
//			LEVEL SCENE
//###########################
Level::Level() : Scene()
{
	
}

void Level::init(ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort)
{
	i = 0;
	//#############SPRITES###########
	//SKAPA EN VANLIG SPRITE
	m_waddaSprite = new SpriteButton(p_d3dDevice, 0, 0, 40, 40, "heart.png", p_viewPort);
	//SÄTT SPRITEN TILL FULLSCREEN
	m_waddaSprite->setAnchorPoint(Sprite::BOTTOMRIGHT);

	m_waddaSprite->setHoverColor(0.0f, 0.0f, 1.0f, 1.0f);
	//###############################

	//Init Shaders
	m_shaderManager = new Shaders();
	m_shaderManager->init(p_d3dDevice);
	m_shaderManager->addShader("Instancing.fx", 12);
	m_shaderManager->addShader("Terrain.fx", 12);
	m_shaderManager->addShader("Obj.fx", 12);
	m_shaderManager->addShader("Pacman.fx", 12);

	//Modelloading
	m_modelManager = new ModelManager(p_d3dDevice);
	m_modelManager->createModel("Wall", "Box.obj");
	m_modelManager->createModel("Candy", "diamond.obj");
	m_modelManager->createModel("PacmanO", "PacO2.obj");
	m_modelManager->createModel("PacmanC", "PacC.obj");
	m_modelManager->createModel("Ghost", "Ghost.obj");
	m_modelManager->createModel("Cherry", "Cherry.obj");

	//Terrain
	m_terrain = new Terrain(p_d3dDevice, 512,512,5);
	

	//WorldLoader
	m_worldLoader = new WorldLoader(p_d3dDevice, m_modelManager);
	m_worldLoader->loadFromFile("MapTest.png", m_terrain->getWidth(), 
								m_terrain->getHeight(), m_terrain->getY(), m_objects);

	//Terrain Init
	m_terrain->initialize((float)m_worldLoader->getTerrainWidth(),(float)m_worldLoader->getTerrainHeight(),m_worldLoader->getTerrainScale(), m_shaderManager->getShaderByName("Terrain.fx"));

	//Instancing
	m_wallInstancing = new Instancing(p_d3dDevice);
	m_wallInstancing->initializeDefault(&m_objects.m_walls, m_shaderManager->getShaderByName("Instancing.fx"));
	m_candyInstancing = new Instancing(p_d3dDevice);
	m_candyInstancing->initializeDynamic(&m_objects.m_candies, m_shaderManager->getShaderByName("Instancing.fx"));
	m_cherryInstancing = new Instancing(p_d3dDevice);
	m_cherryInstancing->initializeDynamic(&m_objects.m_cherries, m_shaderManager->getShaderByName("Instancing.fx"));
	m_powerUpInstancing = new Instancing(p_d3dDevice);
	m_powerUpInstancing->initializeDynamic(&m_objects.m_powerUps, m_shaderManager->getShaderByName("Instancing.fx"));

	//Init All Objects
	m_objects.initialize();
	//Run pacman Init to gain shader
	m_objects.m_pacman->initialize(m_shaderManager->getShaderByName("Pacman.fx"));
	//Run Enemies Init to gain shader
	for each(Enemy* o in m_objects.m_enemies)
	{
		o->initialize(m_shaderManager->getShaderByName("Obj.fx"));
	}

	//Camera and viewports!!

	m_camera = new Camera();
	m_miniMap = new MiniMap();
	m_camera->lookAt(D3DXVECTOR3(-150,25,0), D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f)); 
	float aspect = float(800)/600;
	m_camera->perspective(45, aspect, 0.5f, 1000.0f);
	m_miniMap->initialize(p_d3dDevice);
	m_miniMap->CreateTex();
	p_deltaTime = 0.05f;
	m_testBox = new AABB(D3DXVECTOR3(100,100,100), D3DXVECTOR3(0,0,0), D3DXVECTOR4(1,1,1,1), p_d3dDevice);
	m_testBox2 = new AABB(D3DXVECTOR3(75, 75, 75), D3DXVECTOR3(25, 25, 25), D3DXVECTOR4(1,1,1,1), p_d3dDevice);
}

void Level::draw( ID3DX10Sprite * p_spriteBatch, ID3D10DepthStencilView* p_depthView,ID3D10RenderTargetView* p_renderTarget,D3D10_VIEWPORT p_VP)
{
	
	m_miniMap->prepareRender(p_depthView,p_VP);
	//Write to miniMap
	for each(Enemy* o in m_objects.m_enemies)
	{
		o->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());	
	}
	m_objects.m_pacman->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());
	//m_waddaSprite->draw(p_spriteBatch);
	m_terrain->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());
	m_wallInstancing->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());
	m_candyInstancing->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());
	m_cherryInstancing->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());
	m_powerUpInstancing->render(m_miniMap->getViewMatrix(), m_miniMap->getProjectionMatrix());

	m_miniMap->normalRender(p_depthView, p_renderTarget, p_VP);
	//Render to scene
	//------------------------------DEBUG-------------------------------
	//Render Bounding Volume
	
	for each(Object* o in m_objects.m_walls)
	{
		o->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	}
	
	for each(Object* o in m_objects.m_cherries)
	{
		o->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	}
	
	for each(Object* o in m_objects.m_candies)
	{
		o->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	}
		
	for each(Object* o in m_objects.m_powerUps)
	{
		o->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	}
	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity( &tempMatrix );
	m_testBox->draw( tempMatrix, m_camera->getViewMatrix(), m_camera->getProjectionMatrix() );
	m_testBox2->draw( tempMatrix, m_camera->getViewMatrix(), m_camera->getProjectionMatrix() );

	int test = m_testBox->boxVsBox( m_testBox2 );
	
	//------------------------------DEBUG-------------------------------

	
	for each(Enemy* o in m_objects.m_enemies)
	{
		o->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	}
	m_objects.m_pacman->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	//m_waddaSprite->draw(p_spriteBatch);

	m_terrain->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());

	m_wallInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_candyInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_cherryInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_powerUpInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());

	
	//render minimap
	m_miniMap->finalRender();
}

void Level::update( float p_deltaTime )
{
	m_candyInstancing->updateDynamic(p_deltaTime);
	m_cherryInstancing->updateDynamic(p_deltaTime);
	m_powerUpInstancing->updateDynamic(p_deltaTime);

	m_objects.update(p_deltaTime);

	//m_camera->setCameraPos(m_objects.m_pacman->getPosition(), m_objects.m_pacman->getDirection());
	if (i == 0)
	{
		m_camera->setTarget(m_objects.m_pacman->getPosition());
		m_camera->update(p_deltaTime);
		i++;
	}
}

Level::~Level()
{
	m_terrain->~Terrain();
	m_terrain = NULL;

	m_wallInstancing->~Instancing();
	m_wallInstancing = NULL;
	m_candyInstancing->~Instancing();
	m_candyInstancing = NULL;
	m_cherryInstancing->~Instancing();
	m_cherryInstancing = NULL;
	m_powerUpInstancing->~Instancing();
	m_powerUpInstancing = NULL;

	m_worldLoader->~WorldLoader();
	m_worldLoader = NULL;

	m_objects.DestroyAll();
}

void Level::keyEvent(USHORT key)
{
	float Rotation = 0;
	if(key == 0x41) // A
	{
		//m_camera->strafe( -p_deltaTime );
		m_objects.m_pacman->turnLeft();
		Rotation = -90;
	}
	if(key == 0x44) // D
	{
		//m_camera->strafe( p_deltaTime );
		m_objects.m_pacman->turnRight();
		Rotation = 90;
	}
	if(key == 0x57) // W
	{
		//m_camera->walk( p_deltaTime );
		m_objects.m_pacman->walkF(p_deltaTime);
	}
	if(key == 0x53) // S
	{
		m_camera->rotate(0.0f, 0.0f);
		//m_camera->walk( -p_deltaTime );
		//m_objects.m_pacman->uTurn();
	}
	

	//if(key == 0x52) // R
	//{
	//	m_camera->setY( p_deltaTime * 100 );
	//}
	//else if( key == 0x46) // F
	//{
	//	m_camera->setY( -p_deltaTime * 100 );
	//}
	//else{}

	//DEBUG REMOVE
	if(key == 0x4C) // L
	{
		if(!m_objects.m_candies.empty())
			m_objects.m_candies.pop_back();
	}
	m_camera->rotate(Rotation,0.0f);
	m_camera->setTarget(m_objects.m_pacman->getPosition());
	m_camera->update(p_deltaTime);
}

void Level::leftMouseClick( POINT p_mousePosition )
{

}

void Level::rightMouseClick( POINT p_mousePosition )
{

}

void Level::mouseMove( POINT p_mousePosition )
{
	m_waddaSprite->isButtonHovered(p_mousePosition);
}
