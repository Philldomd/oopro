#pragma once

#include "d3dx9math.h"
#include <D3D10.h>
#include <time.h>



class AI
{
public:
	AI(void);
	~AI(void);
	//matrix of tiles?
	void incrementCandyCounter();
	void setFrightenedMode();
	void eaten();
	D3DXVECTOR2 update(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection, float p_deltaTime);
	virtual D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);
	//kill method? inactive? changedirection? higher speed?

protected:
	int m_candyCounter;
	int m_nrOfCandyToLeaveHouse;
	int m_mode; //scatter, chase, frightened, eaten
	int m_previousMode;
	D3DXVECTOR2 m_direction;
	D3DXVECTOR2 m_currentTile;
	D3DXVECTOR2 m_scatterTile;
	D3DXVECTOR2 m_targetTile;
	D3DXVECTOR2 m_ghostHomeTile;

	enum MODES 
	{
		CHASE,
		SCATTER,
		FRIGHTENED,
		EATEN
	};
	D3DXVECTOR2 UP;
	D3DXVECTOR2 LEFT;
	D3DXVECTOR2 DOWN;
	D3DXVECTOR2 RIGHT;

	float m_timer;
	float m_timerFrightened;
	float m_scatterTime;
	float m_chaseTime;
	float m_frightenedTime;

	bool m_reverseDirection;
	bool m_isActive;

private:
	D3DXVECTOR2 getClosestDirectionToTagetTile();
	bool isAtAnIntersection();
	void changeMode();
};

