#ifndef		PLAYER__H
#define		PLAYER__H

#include "stdafx.h"


enum MOVE{
	LEFT = 0,
	RIGHT
};

class Player
{
public:
	Player();
	~Player();
	void setMove(int p_move);
	void updateScore(int p_score);
	int getScore();
	int getMove();
private:
	int	m_move;
	int	m_score;
};

#endif //PLAYER__H