#pragma once
#include "AI.h"
class BlinkyAI : public AI
{
public:
	BlinkyAI(void);
	~BlinkyAI(void);

	void enableElroyMode(); 

private:
	D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);
};

