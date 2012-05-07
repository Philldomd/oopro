#include "InkyAI.h"


InkyAI::InkyAI(void) : AI()
{
}


InkyAI::~InkyAI(void)
{
}

void InkyAI::setBlinkyPos(D3DXVECTOR2 p_blinkyPos)
{
	m_blinkyPos = p_blinkyPos;
}

D3DXVECTOR2 InkyAI::calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection)
{//direction: up = (0,-1), right = (1,0), down = (0,1), left = (-1,0)
	D3DXVECTOR2 m_tmpVec;
	
	m_tmpVec = p_pacmanPos + p_pacmanDirection * 2;

	if(p_pacmanDirection == UP)
		m_tmpVec.x = p_pacmanPos.x - 2;

	m_tmpVec =  m_tmpVec - m_blinkyPos;
	m_tmpVec *= 2;

	return m_tmpVec;
}