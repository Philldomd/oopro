#include "Scene.h"


Scene::Scene(){}

Scene::~Scene(){}

void Scene::update(float p_deltaTime){}

void Scene::draw(ID3DX10Sprite * p_spriteBatch, ID3D10RenderTargetView* p_renderTarget,
	ID3D10DepthStencilView* p_depthStencil, D3D10_VIEWPORT p_VP){}

void Scene::init( ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort )
{
	m_d3dDevice = p_d3dDevice;
	m_viewPort = p_viewPort;
}

void Scene::keyEvent( USHORT p_key ){}

void Scene::leftMouseClick( POINT p_mousePosition ){}

void Scene::rightMouseClick( POINT p_mousePosition ){}

void Scene::mouseMove( POINT p_mousePosition ){}

