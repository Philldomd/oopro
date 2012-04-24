#include "SceneManager.h"

bool SceneManager::m_instanceFlag = false;
SceneManager* SceneManager::m_sceneManager = NULL;

SceneManager::SceneManager(){}

void SceneManager::initSceneManager(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;

	createBlendState();
	D3DX10CreateSprite(m_d3dDevice, 0, &m_spriteBatch);

	m_primaryScene = NULL;
	m_secondaryScene = NULL;

	m_gamePaused = false;
}

void SceneManager::updateScene( float p_deltaTime )
{
	UINT temp = 1;
	m_d3dDevice->RSGetViewports(&temp, &m_viewPort);

	//Always update m_primaryScene
	m_primaryScene->update(p_deltaTime);

	if(m_secondaryScene && !m_gamePaused)
		m_secondaryScene->update(p_deltaTime);
}

void SceneManager::drawScene()
{
	m_d3dDevice->OMSetBlendState(m_alphaBlendState, 0, 0xFFFFFFFF); 
	m_d3dDevice->OMSetDepthStencilState(0, 0);
	m_spriteBatch->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	//Always draw m_primaryScene
	m_primaryScene->draw(m_spriteBatch);
	if(m_secondaryScene)
		m_secondaryScene->draw(m_spriteBatch);
	m_spriteBatch->End();
}

void SceneManager::removeCurrentScenes()
{
	if(m_primaryScene)
	{
		delete m_primaryScene;
		m_primaryScene = NULL;
	}
	if(m_secondaryScene)
	{
		delete m_secondaryScene;
		m_secondaryScene = NULL;
	}

	m_gamePaused = false;
}

void SceneManager::removeOverlayScene()
{
	delete m_primaryScene;
	m_primaryScene = m_secondaryScene;
	m_secondaryScene = NULL;
	m_gamePaused = false;
}

void SceneManager::addScene( Scene* p_scene )
{
	removeCurrentScenes();
	m_primaryScene = p_scene;
}

void SceneManager::addOverlayScene( Scene* p_scene)
{
	//If there is a secondary scene we know that we can safely replace the 
	//primary scene(which is a overlay scene) with a new overlay scene!
	if(m_secondaryScene)
	{
		delete m_primaryScene;
		m_primaryScene = p_scene;
	}
	else//else we push down the primary scene to secondary and add the overlay scene as primary to be updated and drawn.
	{
		m_secondaryScene = m_primaryScene;
		m_primaryScene = p_scene;
	}
}

void SceneManager::setPauseGame( bool p_paused )
{
	m_gamePaused = p_paused;
}


SceneManager* SceneManager::getInstance()
{
	if(!m_instanceFlag)
	{
		m_sceneManager = new SceneManager();
		m_instanceFlag = true;
		return m_sceneManager;
	}
	else
	{
		return m_sceneManager;
	}
}

void SceneManager::createBlendState()
{
	m_alphaBlendState = NULL;
	D3D10_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D10_BLEND_DESC));
	desc.AlphaToCoverageEnable = TRUE;
	desc.BlendEnable[0] = TRUE;
	desc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	desc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	desc.BlendOp = D3D10_BLEND_OP_ADD;
	desc.SrcBlendAlpha = D3D10_BLEND_SRC_ALPHA;
	desc.DestBlendAlpha = D3D10_BLEND_INV_SRC_ALPHA;
	desc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	desc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	m_d3dDevice->CreateBlendState(&desc, &m_alphaBlendState);
}

void SceneManager::recalculateSpriteBatch()
{
	UINT temp = 1;
	m_d3dDevice->RSGetViewports(&temp, &m_viewPort);

	D3DXMATRIX matProjection;
	D3DXMatrixIdentity(&matProjection);
	// Create the projection matrix using the values in the viewport
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)0,
		(float)m_viewPort.Width,
		(float)0,
		(float)m_viewPort.Height,
		0.1f,
		10);

	m_spriteBatch->SetProjectionTransform(&matProjection);
}

SceneManager::~SceneManager()
{

}

