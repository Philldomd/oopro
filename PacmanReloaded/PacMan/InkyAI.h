#ifndef INKYAI_H
#define INKYAI_H


#include "AI.h"
class InkyAI : public AI
{
public:
	InkyAI(void);
	~InkyAI(void);

	void setBlinkyPos(D3DXVECTOR2 p_blinkyPos);
	D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);

private:
	D3DXVECTOR2 m_blinkyPos;
};

#endif