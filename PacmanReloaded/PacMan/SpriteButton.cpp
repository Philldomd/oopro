#include "SpriteButton.h"

SpriteButton::SpriteButton(ID3D10Device* p_d3dDevice, float p_x, float p_y, float p_width, float p_height, char* p_fileName, D3D10_VIEWPORT* p_viewPort) 
	: Sprite(p_d3dDevice, p_x, p_y, p_width, p_height, p_fileName, p_viewPort)
{
	setHoverColor(1.0f, 0.0f, 0.0f, 1.0f);
	updateBoundingBox();
}

SpriteButton::~SpriteButton()
{
}

void SpriteButton::updateBoundingBox()
{
	m_buttonRect.left	= (long) m_posX;
	m_buttonRect.top	= (long) m_posY;
	m_buttonRect.bottom	= (long)(m_posY + m_height);
	m_buttonRect.right	= (long)(m_posX + m_width);
}

bool SpriteButton::isButtonPressed( POINT p_mousePos )
{
	updateBoundingBox();

	if(p_mousePos.x > m_buttonRect.left		&&
		p_mousePos.x < m_buttonRect.right	&&
		p_mousePos.y > m_buttonRect.top		&&
		p_mousePos.y < m_buttonRect.bottom)
		return true;
	else
		return false;
}

void SpriteButton::isButtonHovered( POINT p_mousePos )
{
	updateBoundingBox();

	if(p_mousePos.x > m_buttonRect.left &&
		p_mousePos.x < m_buttonRect.right &&
		p_mousePos.y > m_buttonRect.top  &&
		p_mousePos.y < m_buttonRect.bottom)
	{
		m_imageSprite.ColorModulate = m_hoverColor;
	}
	else 
		m_imageSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void SpriteButton::setHoverColor( float p_red, float p_green, float p_blue, float p_alpha )
{
	m_hoverColor = D3DXCOLOR(p_red, p_green, p_blue, p_alpha);
}


