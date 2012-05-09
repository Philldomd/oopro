#ifndef PINKYAI_H
#define PINKYAI_H


#include "AI.h"
class PinkyAI : public AI
{
public:
	PinkyAI(void);
	~PinkyAI(void);

	D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);
};

#endif