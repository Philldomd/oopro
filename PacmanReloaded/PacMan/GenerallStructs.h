#ifndef GENERALLSTRUCTS_H
#define GENERALLSTRUCTS_H

#include "Object.h"
#include <vector>

using namespace std;

struct Objects
{
	vector<Object*> m_walls;
	Object*			m_pacman;
	vector<Object*> m_candies;
	vector<Object*> m_enemies;
	vector<Object*> m_cherries;
	vector<Object*> m_powerUps;
};

#endif