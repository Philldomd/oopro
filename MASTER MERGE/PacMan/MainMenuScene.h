#ifndef MENUSCENE
#define MENUSCENE

#include "d3dUtil.h"
#include "Scene.h"
#include "ModelManager.h"
#include "WorldLoader.h"
#include "GenerallStructs.h"
#include "WallInstancing.h"
#include "CandyInstancing.h"

//TEMPORARY STUFF REMOVE !!
#include "Camera.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene(ID3D10Device* p_d3dDevice);
	~MainMenuScene();

	void  draw(ID3DX10Sprite * p_spriteBatch);
	void  update(float p_deltaTime);

private:
	ModelManager*	m_modelManager;
	WorldLoader*	m_worldLoader;
	Objects			m_objects;

	//Drawing with Instancing
	WallInstancing* m_wallInstancing;
	CandyInstancing* m_candyInstancing;

	//TEMPORARY STUFF REMOVE !!
	Camera*		m_camera;
};

#endif