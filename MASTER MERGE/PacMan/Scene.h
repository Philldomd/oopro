#ifndef SCENE_
#define SCENE_

#include "d3dUtil.h"
#include "Player.h"
#include <vector>

class Scene
{
public:

	Scene();
	virtual ~Scene();
	virtual void  init(ID3D10Device* p_d3dDevice, D3D10_VIEWPORT* p_viewPort);
	virtual void  draw(ID3DX10Sprite * p_spriteBatch,ID3D10DepthStencilView* p_depthView,ID3D10RenderTargetView* p_renderTarget,D3D10_VIEWPORT p_VP);
	virtual void  update(float p_deltaTime);
	virtual void  keyEvent(USHORT p_key);
	virtual void  leftMouseClick(POINT p_mousePosition);
	virtual void  rightMouseClick(POINT p_mousePosition);
	virtual void  mouseMove( POINT p_mousePosition );
	virtual void  setPlayer(Player* p_player);

protected:	

	ID3D10Device*	m_d3dDevice;
	D3D10_VIEWPORT* m_viewPort;
	Player*			m_player;
	bool			debug;

};

#endif