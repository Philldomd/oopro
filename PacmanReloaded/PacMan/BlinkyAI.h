#ifndef BLINKYAI_H
#define BLINKYAI_H


#include "AI.h"
class BlinkyAI : public AI
{
public:
	BlinkyAI(void);
	~BlinkyAI(void);

	void enableElroyMode(); 

	D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);
};

#endif