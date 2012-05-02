#ifndef SPRITEBUTTON_
#define SPRITEBUTTON_

#include "d3dUtil.h"
#include "Sprite.h"
#include <vector>
#include <string>

using namespace std;


class SpriteButton : public Sprite
{
public:

	SpriteButton(ID3D10Device* p_d3dDevice, float p_x, float p_y, float p_width, float p_height, char* p_fileName, D3D10_VIEWPORT* p_viewPort);
	~SpriteButton();

	void setHoverColor(float p_red, float p_green, float p_blue, float p_alpha);
	bool isButtonPressed(POINT p_mousePos);
	void isButtonHovered(POINT p_mousePos);

private:

	void updateBoundingBox();

private:

	RECT		m_buttonRect;
	D3DXCOLOR	m_hoverColor;



};
#endif