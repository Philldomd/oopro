#ifndef MENUSCENE
#define MENUSCENE

#include "d3dUtil.h"
#include "Scene.h"
#include "ModelManager.h"
#include "WorldLoader.h"
#include "GeneralStructs.h"
#include "Instancing.h"
#include "SpriteButton.h"
#include "Terrain.h"
#include "Shaders.h"

//Camera & viewports

#include "Camera.h"
#include "Minimap.h"

class Level : public Scene
{
public:
	Level();
	~Level();

	void	init(ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort);
	void	draw(ID3DX10Sprite * p_spriteBatch,ID3D10DepthStencilView* p_depthView,ID3D10RenderTargetView* p_renderTarget,D3D10_VIEWPORT p_VP);
	void	update(float p_deltaTime);
	void	keyEvent(USHORT key);
	void	leftMouseClick(POINT p_mousePosition);
	void	rightMouseClick(POINT p_mousePosition);
	void	mouseMove(POINT p_mousePosition);

private:
	ModelManager*	m_modelManager;
	WorldLoader*	m_worldLoader;
	Objects			m_objects;
	float			p_deltaTime;
	Terrain*		m_terrain;
	//ShaderManager
	Shaders*		m_shaderManager;
	int i;
	//Drawing with Instancing
	Instancing* m_wallInstancing;
	Instancing* m_candyInstancing;
	Instancing* m_cherryInstancing;
	Instancing* m_powerUpInstancing;


	//!!
	Camera*		m_camera;
	MiniMap*	m_miniMap;
	//Test Sprite
	SpriteButton* m_waddaSprite;
};

#endif