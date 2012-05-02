#include "ClydeAI.h"


ClydeAI::ClydeAI(void) : AI()
{
}


ClydeAI::~ClydeAI(void)
{
}


D3DXVECTOR2 ClydeAI::calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection)
{
	if(D3DXVec2Length(&(p_pacmanPos - m_currentTile)) < 8.0f)
		return p_pacmanPos;
	else
		return D3DXVECTOR2(-1, -1);;
}