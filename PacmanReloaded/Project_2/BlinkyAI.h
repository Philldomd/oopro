#pragma once
#include "AI.h"
class BlinkyAI : public AI
{
public:
	BlinkyAI(void);
	~BlinkyAI(void);

	void enableElroyMode(); //???
	void setElroySpeed(float p_speed);
	float getElroySpeed();

private:
	bool m_elroyMode;
	float m_elroySpeed;

	D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);
};

