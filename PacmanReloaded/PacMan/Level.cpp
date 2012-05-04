#include "Level.h"

//###########################
//			LEVEL SCENE
//###########################
Level::Level() : Scene()
{
	
}

void Level::init(ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort)
{

	//#############SPRITES###########
	//SKAPA EN VANLIG SPRITE
	m_waddaSprite = new SpriteButton(p_d3dDevice, 0, 0, 40, 40, "heart.png", p_viewPort);
	//SÄTT SPRITEN TILL FULLSCREEN
	m_waddaSprite->setAnchorPoint(Sprite::BOTTOMRIGHT);

	m_waddaSprite->setHoverColor(0.0f, 0.0f, 1.0f, 1.0f);
	//###############################

	//FX TEMP
	fx::InitAll(p_d3dDevice);

	//Modelloading
	m_modelManager = new ModelManager(p_d3dDevice);
	m_modelManager->createModel("Wall", "Box.obj");
	m_modelManager->createModel("Candy", "diamond.obj");
	//m_modelManager->createModel("Pacman", "PacMan_Open.obj");
	m_modelManager->createModel("Cherry", "Cherry.obj");

	//Terrain
	m_terrain = new Terrain(p_d3dDevice, 512,512,5);
	

	//WorldLoader
	m_worldLoader = new WorldLoader(p_d3dDevice, m_modelManager);
	m_worldLoader->loadFromFile("MapTest.png", m_terrain->getWidth(), 
								m_terrain->getHeight(), m_terrain->getY(), m_objects);

	//Terrain Init
	m_terrain->initialize((float)m_worldLoader->getTerrainWidth(),(float)m_worldLoader->getTerrainHeight(),m_worldLoader->getTerrainScale());

	//Instancing
	m_wallInstancing = new WallInstancing(p_d3dDevice);
	m_wallInstancing->initialize(&m_objects.m_walls);
	m_candyInstancing = new CandyInstancing(p_d3dDevice);
	m_candyInstancing->initialize(&m_objects.m_candies);
	m_cherryInstancing = new CandyInstancing(p_d3dDevice);
	m_cherryInstancing->initialize(&m_objects.m_cherries);
	m_powerUpInstancing = new CandyInstancing(p_d3dDevice);
	m_powerUpInstancing->initialize(&m_objects.m_powerUps);

	//Init All Objects
	m_objects.initialize();

	//TEMPORARY STUFF REMOVE !!
	m_camera = new Camera(D3DXVECTOR3( 200, 14, 200 ));
	float aspect = float(800)/600;
	m_camera->createProjectionMatrix(0.25f*PI, aspect, 0.5f, 1000.0f);

	p_deltaTime = 0.05f;
}

void Level::draw( ID3DX10Sprite * p_spriteBatch )
{
	
	m_waddaSprite->draw(p_spriteBatch);

	m_terrain->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());

	m_wallInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_candyInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_cherryInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
	m_powerUpInstancing->render(m_camera->getViewMatrix(), m_camera->getProjectionMatrix());

	
}

void Level::update( float p_deltaTime )
{


	m_candyInstancing->update(p_deltaTime);
	m_cherryInstancing->update(p_deltaTime);
	m_powerUpInstancing->update(p_deltaTime);

	m_objects.update(p_deltaTime);

	//m_camera->setCameraPos(m_objects.m_pacman->getPosition(), m_objects.m_pacman->getDirection());
	m_camera->updateCameraPos();
	m_camera->updateViewMatrix();
}

Level::~Level()
{
	m_terrain->~Terrain();
	m_terrain = NULL;

	m_wallInstancing->~WallInstancing();
	m_wallInstancing = NULL;
	m_candyInstancing->~CandyInstancing();
	m_candyInstancing = NULL;
	m_cherryInstancing->~CandyInstancing();
	m_cherryInstancing = NULL;
	m_powerUpInstancing->~CandyInstancing();
	m_powerUpInstancing = NULL;

	m_worldLoader->~WorldLoader();
	m_worldLoader = NULL;

	m_objects.DestroyAll();

	fx::DestroyAll();
}

void Level::keyEvent(USHORT key)
{
	if(key == 0x41) // A
	{
		m_camera->strafe( -p_deltaTime );
		//m_objects.m_pacman->turnLeft();
	}
	if(key == 0x44) // D
	{
		m_camera->strafe( p_deltaTime );
		//m_objects.m_pacman->turnRight();
	}
	if(key == 0x57) // W
	{
		m_camera->walk( p_deltaTime );
	}
	if(key == 0x53) // S
	{
		m_camera->walk( -p_deltaTime );
		//m_objects.m_pacman->uTurn();
	}
	

	if(key == 0x52) // R
	{
		m_camera->setY( p_deltaTime * 100 );
	}
	else if( key == 0x46) // F
	{
		m_camera->setY( -p_deltaTime * 100 );
	}
	else{}

	//DEBUG REMOVE
	if(key == 0x4C) // L
	{
		if(!m_objects.m_candies.empty())
			m_objects.m_candies.pop_back();
	}
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
