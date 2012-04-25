#include "AI.h"


AI::AI(void)
{
	//direction m.m.
	UP = D3DXVECTOR2(0, -1);
	LEFT = D3DXVECTOR2(-1, 0);
	DOWN = D3DXVECTOR2(0, 1);
	RIGHT = D3DXVECTOR2(1, 0);
}


AI::~AI(void)
{
	//asdf
}

void AI::incrementCandyCounter()
{
	m_candyCounter++;
}

D3DXVECTOR2 AI::update(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection, float p_deltaTime)
{
	//update currentTile

	if(m_mode == EATEN)
	{
		m_timer += p_deltaTime;
		if(isAtAnIntersection())
		{
			m_direction = getClosestDirectionToTagetTile();
		}
		if(m_currentTile == m_ghostHomeTile)
		{
			m_mode = m_previousMode;
			if(m_mode == SCATTER)
			{
				m_timer = m_scatterTime;
				m_targetTile = m_scatterTile;
			}
			else if(m_mode == CHASE)
			{
				m_timer = m_chaseTime;
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
			if(isAtAnIntersection())
			{
				D3DXVECTOR2 m_tmpDir = m_direction;
				while(m_direction == m_tmpDir || (m_direction.x == 0 && m_direction.y == 0))
				{
					srand(time(NULL));
					m_direction.x = rand() % 3 - 1;
					if(m_direction.x != 0)
						m_direction.y = 0;
					else
						m_direction.y = rand() % 3 - 1;
				}
			}
		}
		else if(m_mode != FRIGHTENED)
		{
			m_timer += p_deltaTime;
		}
		if(m_candyCounter == m_nrOfCandyToLeaveHouse)
		{
			m_isActive = true;
			calculateTargetTile(p_pacmanPos, p_pacmanDirection);
		}
		if(m_isActive)
		{
			if(isAtAnIntersection())
			{
				m_direction = getClosestDirectionToTagetTile();
			}
			if(m_timer <= 0)
			{
				changeMode();
				m_reverseDirection = true;
				if(m_mode == SCATTER)
				{
					m_timer = m_scatterTime;
					m_targetTile = m_scatterTile;
				}
				else if(m_mode == CHASE)
				{
					m_timer = m_chaseTime;
					m_targetTile = calculateTargetTile(p_pacmanPos, p_pacmanDirection);
					if(m_targetTile == D3DXVECTOR2(-1, -1))
						m_targetTile = m_scatterTile;
				}
			}
		}

		if(m_reverseDirection && m_isActive)
		{
			m_direction *= -1;
			m_reverseDirection = false;
		}
	}
	return m_direction;
}
//check which paths are valid, cannot turn back or into a wall!
D3DXVECTOR2 AI::getClosestDirectionToTagetTile() 
{
	D3DXVECTOR2 m_tmpDir = m_direction;
	D3DXVECTOR2 m_tmpTile;
	float m_distance = INT_MAX;
	if((m_direction*-1) != UP) //bryta ut till metoder?
	{
		m_tmpTile = m_currentTile + UP;
		if(D3DXVec2Length(&(m_targetTile - m_tmpTile)) < m_distance)
		{
			m_distance = D3DXVec2Length(&(m_targetTile - m_tmpTile));
			m_direction = UP;
		}
	}
	if((m_direction*-1) != LEFT)
	{
		m_tmpTile = m_currentTile + LEFT;
		if(D3DXVec2Length(&(m_targetTile - m_tmpTile)) < m_distance)
		{
			m_distance = D3DXVec2Length(&(m_targetTile - m_tmpTile));
			m_direction = LEFT;
		}
	}
	if((m_direction*-1) != DOWN)
	{
		m_tmpTile = m_currentTile + DOWN;
		if(D3DXVec2Length(&(m_targetTile - m_tmpTile)) < m_distance)
		{
			m_distance = D3DXVec2Length(&(m_targetTile - m_tmpTile));
			m_direction = DOWN;
		}
	}
	if((m_direction*-1) != RIGHT)
	{
		m_tmpTile = m_currentTile + RIGHT;
		if(D3DXVec2Length(&(m_targetTile - m_tmpTile)) < m_distance)
		{
			m_distance = D3DXVec2Length(&(m_targetTile - m_tmpTile));
			m_direction = RIGHT;
		}
	}
	return m_tmpDir;
}

bool AI::isAtAnIntersection()
{
	//if(m_currentTile == m_intersectionTile) MÅSTE VARA I MITTEN AV EN TILE
	return true;
}

void AI::changeMode()
{
	if(m_mode == CHASE)
		m_mode = SCATTER;
	else if(m_mode == SCATTER)
		m_mode = CHASE;
}

void AI::eaten()
{
	m_previousMode = m_mode;
	m_mode = EATEN;
	m_targetTile = m_ghostHomeTile;
	//setSpeed(faster);
}

void AI::setFrightenedMode()
{
	m_reverseDirection = true;
	m_mode = FRIGHTENED;
}

D3DXVECTOR2 calculateTargetTile(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection) {	}
