#include "Sprite.h"

Sprite::Sprite(ID3D10Device* p_d3dDevice, float p_x, float p_y, float p_width, float p_height, char* p_fileName, D3D10_VIEWPORT* p_viewPort)
	
{
	m_d3dDevice = p_d3dDevice;
	m_viewPort = p_viewPort;
	
	// Set the sprite's shader resource view
	m_imageSprite.pTexture = createTex(p_fileName);

	m_x = p_x;
	m_y = p_y;

	m_posX = m_x;
	m_posY = m_y;

	m_height = p_height;
	m_width = p_width;

	// top-left location in U,V coords
	m_imageSprite.TexCoord.x = 0;
	m_imageSprite.TexCoord.y = 0;

	// Determine the texture size in U,V coords
	m_imageSprite.TexSize.x = 1.0f;
	m_imageSprite.TexSize.y = 1.0f;

	// Set the texture index. Single textures will use 0
	m_imageSprite.TextureIndex = 0;

	m_imageSprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXMatrixIdentity(&m_matTranslate);
	D3DXMatrixIdentity(&m_matScale);

	D3DXMatrixScaling(&m_matScale, m_width, m_height, 1.0f);
	
	m_anchorPoint = NORMAL;

	m_centerX = false;
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void Sprite::setScale(float scale)
{
	m_width = m_width * scale;
	m_height = m_height * scale;

	D3DXMatrixScaling(&m_matScale, m_width, m_height, 1.0f);

}

ID3D10ShaderResourceView* Sprite::createTex(char* p_filename)
{
	char stringBuffer[MAX_PATH];
	GetModuleFileName( NULL, stringBuffer, MAX_PATH );
	std::string::size_type charPosition = string( stringBuffer ).find_last_of( "\\/" );
	std::string baseDirectory = string( stringBuffer ).substr( 0, charPosition);
	charPosition = baseDirectory.find_last_of( "\\/" );
	baseDirectory = baseDirectory.substr( 0, charPosition);

	string resourceFolder = baseDirectory.append("\\PacMan\\Resources\\Sprites\\");
	resourceFolder = resourceFolder.append(p_filename);

	ID3D10ShaderResourceView* rv = 0;
	D3DX10CreateShaderResourceViewFromFile(m_d3dDevice, resourceFolder.c_str(), 0, 0, &rv, 0 );

	return rv;
}


void Sprite::draw(ID3DX10Sprite *spriteObject)
{
	
	switch(m_anchorPoint)
	{
	case NORMAL:
		break;
	case TOPLEFT:
		m_posX = m_x;
		m_posY = m_y;
		break;
	case TOPRIGHT:
		m_posX = (m_viewPort->Width - m_x) - m_width;
		m_posY = m_y;
		break;
	case BOTTOMLEFT:
		m_posX = m_x;
		m_posY = (m_viewPort->Height- m_y) - m_height;
		break;
	case BOTTOMRIGHT:
		m_posX = m_viewPort->Width - m_x - m_width;
		m_posY = m_viewPort->Height- m_y - m_height;
		break;
	case FULLSCREEN:
		m_height = (float)m_viewPort->Height;
		m_width = (float)m_viewPort->Width;
		D3DXMatrixScaling(&m_matScale, m_width, m_height, 1.0f );
		m_posX = 0;
		m_posY = 0;
		break;
	case CENTER:
		m_posX = m_viewPort->Width*0.5f - m_width*0.5f;
		m_posY = m_viewPort->Height*0.5f - m_height*0.5f;
		break;
	}

	if(m_centerX)
	{
		m_posX = m_viewPort->Width*0.5f - m_width*0.5f;
	}
	
	D3DXMatrixTranslation( &m_matTranslate, m_posX + m_width*0.5f, (m_viewPort->Height - m_posY) - m_height*0.5f , 0.1f);
	m_imageSprite.matWorld = (m_matScale * m_matTranslate);
	spriteObject->DrawSpritesImmediate(&m_imageSprite, 1, 0, 0);
	//spriteObject->DrawSpritesBuffered(&m_imageSprite, 1);
	
}

void Sprite::lockToCenterX(bool p_lockToCenterX)
{
	m_centerX = p_lockToCenterX;
}

void Sprite::setAnchorPoint( LOCKSTATE p_anchorPoint )
{
	m_anchorPoint = p_anchorPoint;
}
