#ifndef SPRITE_
#define SPRITE_

#include "d3dUtil.h"
#include <vector>
#include <string>

using namespace std;


class Sprite
{
public:

	Sprite(ID3D10Device* p_d3dDevice, float p_x, float p_y, float p_width, float p_height, char* p_fileName, D3D10_VIEWPORT* p_viewPort);
	~Sprite();

	enum LOCKSTATE {NORMAL, CENTER, TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT, FULLSCREEN} m_anchorPoint;

	void draw(ID3DX10Sprite* p_spriteBatch);
	void setPosition(float p_x, float p_y);
	void setScale(float p_scale);
	void lockToCenterX(bool p_lockTocenterX);
	void setAnchorPoint(LOCKSTATE p_anchorPoint);

private:

	ID3D10ShaderResourceView* createTex(char * p_fileName);
	
protected:

	ID3D10Device* m_d3dDevice;

	D3DX10_SPRITE m_imageSprite;

	D3DXMATRIX m_matTranslate;
	D3DXMATRIX m_matScale;

	float m_x, m_y, m_width, m_height, m_posX, m_posY;

	D3D10_VIEWPORT * m_viewPort;

	bool m_centerX;


};
#endif