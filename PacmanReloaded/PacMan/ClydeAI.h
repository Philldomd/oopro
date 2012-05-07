#ifndef CLYDEAI_H
#define CLYDEAI_H


#include "AI.h"
class ClydeAI : public AI
{
public:
	ClydeAI(void);
	~ClydeAI(void);

	D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);
};

#endif