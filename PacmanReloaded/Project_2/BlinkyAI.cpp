#include "BlinkyAI.h"


BlinkyAI::BlinkyAI(void)
{
	m_elroyMode = false;
}


BlinkyAI::~BlinkyAI(void)
{
}


D3DXVECTOR2 BlinkyAI::calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection)
{
	return p_pacmanPos;
}

void BlinkyAI::enableElroyMode()
{
	m_elroyMode = true;
	//speed = elroySpeed;
}