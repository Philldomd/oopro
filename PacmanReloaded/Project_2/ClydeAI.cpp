#include "ClydeAI.h"


ClydeAI::ClydeAI(void)
{
}


ClydeAI::~ClydeAI(void)
{
}


D3DXVECTOR2 ClydeAI::calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection)
{
	D3DXVECTOR2 m_tmpVec;

	m_tmpVec = p_pacmanPos - m_currentTile;

	if(D3DXVec2Length(&m_tmpVec) < 8.0f)
		return p_pacmanPos;
	else
	{
		m_tmpVec = D3DXVECTOR2(-1, -1);
		return m_tmpVec;
	}
}