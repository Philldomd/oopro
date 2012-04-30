#include "Scene.h"


Scene::Scene(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;
}

Scene::~Scene(){}

void Scene::update(float p_deltaTime){}

void Scene::draw(ID3DX10Sprite * p_spriteBatch){}

