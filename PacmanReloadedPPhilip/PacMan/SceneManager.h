#ifndef SCENEMANAGER_
#define SCENEMANAGER_

#include "Scene.h"

using namespace std;

class SceneManager
{
public:
	~SceneManager();

	static SceneManager* getInstance();

	void updateScene(float p_deltaTime);
	void drawScene(ID3D10RenderTargetView* p_renderTarget,
		ID3D10DepthStencilView* p_depthStencil, D3D10_VIEWPORT p_VP);

	void initSceneManager(ID3D10Device* p_d3dDevice);
	void removeCurrentScenes();
	void removeOverlayScene();
	void addScene(Scene* p_scene);
	void addOverlayScene(Scene* p_scene);
	void setPauseGame(bool p_paused);

	void recalculateSpriteBatch();

public:

private:
	SceneManager();
	void createBlendState();
	void initScene(Scene* p_scene);

private:
	static bool				m_instanceFlag;
	static SceneManager*	m_sceneManager;
	ID3D10Device*			m_d3dDevice;
	Scene*					m_primaryScene;
	Scene*					m_secondaryScene;
	bool					m_gamePaused;
	D3D10_VIEWPORT			m_viewPort;
	ID3D10BlendState*		m_alphaBlendState;
	ID3DX10Sprite*			m_spriteBatch;

};
#endif