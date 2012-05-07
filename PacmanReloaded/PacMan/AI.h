#ifndef AI_H
#define AI_H

#include "d3dx9math.h"
#include <D3D10.h>
#include <time.h>
#include <vector>

using namespace std;

class AI
{
public:
	AI(void);
	virtual ~AI(void);

	void init(int p_nrOfCandiesToLeaveHouse, int p_nrOfCandiesForElroyMode1, int p_nrOfCandiesForElroyMode2, 
		float p_scatterTimes[4], float p_chaseTimes[4], float p_frightenedTime, vector<vector<char>> p_tileMatrix, 
		D3DXVECTOR2 p_scatterTile, D3DXVECTOR2 p_ghostHomeTile);

	void incrementCandyCounter();
	void setFrightenedMode();
	void eaten();
	D3DXVECTOR2 update(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection, D3DXVECTOR2 p_ghostPos, float p_deltaTime);
	virtual D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection);

protected:
	int m_candyCounter;
	int m_nrOfCandiesToLeaveHouse;
	int m_nrOfCandiesForElroyMode1;
	int m_nrOfCandiesForElroyMode2;
	int m_mode; //scatter, chase, frightened
	int m_previousMode;
	int m_currentTimerModeNr;

	D3DXVECTOR2 m_direction;
	D3DXVECTOR2 m_currentTile;
	D3DXVECTOR2 m_scatterTile;
	D3DXVECTOR2 m_targetTile;
	D3DXVECTOR2 m_ghostHomeTile;

	vector<vector<char>> m_tileMatrix;

	enum MODES 
	{
		CHASE,
		SCATTER,
		FRIGHTENED
	};// m_mode, m_previousMode;

	D3DXVECTOR2 UP;
	D3DXVECTOR2 LEFT;
	D3DXVECTOR2 DOWN;
	D3DXVECTOR2 RIGHT;

	float m_timer;
	float m_timerFrightened;
	float m_scatterTimes[4];
	float m_chaseTimes[4];
	float m_frightenedTime;

	float* m_speed; //kolla upp (speedmodifier?)
	float m_originalSpeed; //set to speed

	bool m_isEaten;
	bool m_reverseDirection;
	bool m_isActive;
	bool m_elroyMode;

private:
	void setDirectionClosestToTagetTile();
	float getDistanceFromIntersectionTiles(D3DXVECTOR2 p_direction, float p_distance);
	bool isAtAnIntersection();
	void changeMode();
	virtual void enableElroyMode();
};

#endif