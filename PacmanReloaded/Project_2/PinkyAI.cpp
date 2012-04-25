#include "PinkyAI.h"


PinkyAI::PinkyAI(void)
{
}


PinkyAI::~PinkyAI(void)
{
}

D3DXVECTOR2 PinkyAI::calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection)
{//direction: up = (0,-1), right = (1,0), down = (0,1), left = (-1,0)
	D3DXVECTOR2 m_tmpVec;

	m_tmpVec = p_pacmanPos + p_pacmanDirection * 4;

	if(p_pacmanDirection.y == -1)
		m_tmpVec.x = p_pacmanPos.x - 4;

	return m_tmpVec;
}
