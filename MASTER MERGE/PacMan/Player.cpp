#include "Player.h"


Player::Player()
{
	m_score = 0;
	m_move = -1;
}
Player::~Player(){}

void Player::setMove(int p_move)
{
	m_move = p_move;
}

void Player::updateScore(int p_score)
{
	m_score = p_score;
}

int Player::getMove()
{
	return m_move;
}

int Player::getScore()
{
	return m_score;
}