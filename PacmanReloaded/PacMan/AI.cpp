#include "AI.h"

AI::AI(void)
{
}

AI::~AI(void)
{
	//fixa!
	m_speed = NULL;
}

void AI::init(int p_nrOfCandiesToLeaveHouse, int p_nrOfCandiesForElroyMode1, int p_nrOfCandiesForElroyMode2, 
	float p_scatterTimes[4], float p_chaseTimes[4], float p_frightenedTime, vector<vector<char>> p_tileMatrix, 
	D3DXVECTOR2 p_scatterTile, D3DXVECTOR2 p_ghostHomeTile)
{
	m_candyCounter = 0;
	m_nrOfCandiesToLeaveHouse = p_nrOfCandiesToLeaveHouse;
	m_nrOfCandiesForElroyMode1 = p_nrOfCandiesForElroyMode1;
	m_nrOfCandiesForElroyMode2 = p_nrOfCandiesForElroyMode2;
	
	for(int i = 0; i < 4; i++)
	{
		m_scatterTimes[i] = p_scatterTimes[i];
		m_chaseTimes[i] = p_chaseTimes[i];
	}
	
	m_timerFrightened = 0;
	m_frightenedTime = p_frightenedTime;
	m_timer = m_scatterTimes[0]; 
	m_mode = SCATTER; 

	m_tileMatrix = p_tileMatrix;
	m_scatterTile = p_scatterTile;
	m_ghostHomeTile = p_ghostHomeTile;

	m_reverseDirection = false;
	m_isActive = false; 
	m_elroyMode = false;

	UP = D3DXVECTOR2(0, -1);
	LEFT = D3DXVECTOR2(-1, 0);
	DOWN = D3DXVECTOR2(0, 1);
	RIGHT = D3DXVECTOR2(1, 0);
}



void AI::incrementCandyCounter()
{
	m_candyCounter++;

	if(m_candyCounter == m_nrOfCandiesForElroyMode1)
		enableElroyMode(); //enable elroy mode 1, Blinkys speed increased by 0.05f
	else if(m_candyCounter == m_nrOfCandiesForElroyMode2)
		enableElroyMode(); //enable elroy mode 2, Blinkys speed increased by 0.05f again
}

D3DXVECTOR2 AI::update(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection, D3DXVECTOR2 p_ghostPos, float p_deltaTime)
{
	m_currentTile = p_ghostPos;

	if(m_candyCounter == m_nrOfCandiesToLeaveHouse) //leave house in order. When pacman dies?
	{
		m_isActive = true;
		if(m_mode == SCATTER)
		{
			m_targetTile = m_scatterTile;
		}
		else if(m_mode == CHASE)
		{
			m_targetTile = calculateTargetTile(p_pacmanPos, p_pacmanDirection);
			if(m_targetTile == D3DXVECTOR2(-1, -1))
				m_targetTile = m_scatterTile;
		}
	}

	if(m_mode != FRIGHTENED && !m_isActive)
	{
		m_timer -= p_deltaTime;
		if(m_timer <= 0)
		{
			changeMode();
			m_reverseDirection = true;
			if(m_mode == SCATTER)
				m_timer += m_scatterTimes[m_currentTimerModeNr];
			else if(m_mode == CHASE)
				m_timer += m_chaseTimes[m_currentTimerModeNr];
		}
	}
	
	if(m_isEaten)
	{
		m_timerFrightened -= p_deltaTime;
		if(m_timerFrightened <= 0)
		{
			m_mode = m_previousMode;
		}
		if(isAtAnIntersection())
		{
			setDirectionClosestToTagetTile();
		}
		if(m_currentTile == m_ghostHomeTile)
		{
			m_isEaten = false;
			*m_speed = m_originalSpeed;
			if(m_mode == SCATTER)
			{
				m_targetTile = m_scatterTile;
			}
			else if(m_mode == CHASE)
			{
				m_targetTile = calculateTargetTile(p_pacmanPos, p_pacmanDirection);
				if(m_targetTile == D3DXVECTOR2(-1, -1))
					m_targetTile = m_scatterTile;
			}
		}
	}
	else
	{
		if(m_mode == FRIGHTENED)
		{
			m_timerFrightened -= p_deltaTime;
			if(m_timerFrightened <= 0)
			{
				m_mode = m_previousMode;
				*m_speed = m_originalSpeed;
			}
			if(isAtAnIntersection())
			{
				D3DXVECTOR2 m_tmpDir = m_direction;
				while((m_direction*-1) == m_tmpDir || (m_tmpDir.x == 0 && m_tmpDir.y == 0) || 
					m_tileMatrix.at(m_currentTile.x + m_tmpDir.x).at(m_currentTile.y + m_tmpDir.y) != 'W') 
				{
					srand(time(NULL));
					m_tmpDir.x = rand() % 3 - 1;
					if(m_tmpDir.x != 0)
						m_tmpDir.y = 0;
					else
						m_tmpDir.y = rand() % 3 - 1;
				}
				m_direction = m_tmpDir;
			}
		}
		
		if(m_isActive)
		{
			m_timerFrightened -= p_deltaTime;
			if(isAtAnIntersection())
			{
				setDirectionClosestToTagetTile();
			}
			if(m_timer <= 0)
			{
				changeMode();
				m_reverseDirection = true;
				if(m_mode == SCATTER)
				{
					m_timer += m_scatterTimes[m_currentTimerModeNr];
					if(m_elroyMode)
						m_targetTile = calculateTargetTile(p_pacmanPos, p_pacmanDirection);
					else
						m_targetTile = m_scatterTile;
				}
				else if(m_mode == CHASE)
				{
					m_timer += m_chaseTimes[m_currentTimerModeNr];
					m_targetTile = calculateTargetTile(p_pacmanPos, p_pacmanDirection);
					if(m_targetTile == D3DXVECTOR2(-1, -1))
						m_targetTile = m_scatterTile;
				}
			}
		}

		else
		{
			m_direction = D3DXVECTOR2(0, 0);
		}

		if(m_reverseDirection && m_isActive)
		{
			m_direction *= -1;
			m_reverseDirection = false;
		}
	}
	return m_direction;
}

void AI::setDirectionClosestToTagetTile() 
{
	D3DXVECTOR2 m_tmpTile;
	float m_distance = INT_MAX;

	m_distance = getDistanceFromIntersectionTiles(UP, m_distance);
	m_distance = getDistanceFromIntersectionTiles(LEFT, m_distance);
	m_distance = getDistanceFromIntersectionTiles(DOWN, m_distance);
	m_distance = getDistanceFromIntersectionTiles(RIGHT, m_distance);
}

float AI::getDistanceFromIntersectionTiles(D3DXVECTOR2 p_direction, float p_distance)
{
	D3DXVECTOR2 m_tmpTile = m_currentTile + p_direction;
	if((m_direction*-1) != p_direction && m_tileMatrix.at(m_tmpTile.x).at(m_tmpTile.y) != 'W')
	{
		if(D3DXVec2Length(&(m_targetTile - m_tmpTile)) < p_distance)
		{
			if(!(p_direction == UP && m_tileMatrix.at(m_currentTile.x).at(m_currentTile.y) == 'T')) 
			{
				p_distance = D3DXVec2Length(&(m_targetTile - m_tmpTile));
				m_direction = p_direction;
			}
		}
	}
	return p_distance;
}

bool AI::isAtAnIntersection()
{
	if(m_tileMatrix.at(m_currentTile.x).at(m_currentTile.y) != 'C') 
		return true;
	else
		return false;
}

void AI::changeMode()
{
	if(m_currentTimerModeNr < 4)
		m_currentTimerModeNr++;
	if(m_mode == CHASE)
		m_mode = SCATTER;
	else if(m_mode == SCATTER)
		m_mode = CHASE;
}

void AI::enableElroyMode() {	}

void AI::eaten()
{
	m_isEaten = true;
	m_targetTile = m_ghostHomeTile;
	*m_speed = 1.5f;
}

void AI::setFrightenedMode()
{
	if(!m_isEaten)
	{
		m_reverseDirection = true;
		m_previousMode = m_mode;
		m_mode = FRIGHTENED;
	}
}

D3DXVECTOR2 AI::calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection) 
{
	return D3DXVECTOR2(-1, -1);
}