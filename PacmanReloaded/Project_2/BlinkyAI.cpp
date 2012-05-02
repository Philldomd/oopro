#include "BlinkyAI.h"


BlinkyAI::BlinkyAI(void) : AI()
{
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
	*m_speed += 0.05f;
	m_originalSpeed = *m_speed;
}