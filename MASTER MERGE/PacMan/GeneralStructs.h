#ifndef GENERALSTRUCTS_H
#define GENERALSTRUCTS_H

#include "Object.h"
#include "Pacman.h"
#include <vector>

using namespace std;

struct Objects
{
	vector<Object*> m_walls;
	Pacman*			m_pacman;
	vector<Object*> m_candies;
	vector<Object*> m_enemies;
	vector<Object*> m_cherries;
	vector<Object*> m_powerUps;

	void DestroyAll()
	{
		//Destroy Walls
		for each(Object* o in m_walls)
		{
			o->~Object();
			o = NULL;
		}
		m_walls.clear();
		m_walls.shrink_to_fit();
		//Destroy candies
		for each(Object* o in m_candies)
		{
			o->~Object();
			o = NULL;
		}
		m_candies.clear();
		m_candies.shrink_to_fit();
		//Destroy cherries
		for each(Object* o in m_cherries)
		{
			o->~Object();
			o = NULL;
		}
		m_cherries.clear();
		m_cherries.shrink_to_fit();
		//Destroy powerUps
		for each(Object* o in m_powerUps)
		{
			o->~Object();
			o = NULL;
		}
		m_powerUps.clear();
		m_powerUps.shrink_to_fit();
		//Destroy enemies
		for each(Object* o in m_enemies)
		{
			o->~Object();
			o = NULL;
		}
		m_enemies.clear();
		m_enemies.shrink_to_fit();
		//Destroy Pacman
		m_pacman->~Pacman();
		m_pacman = NULL;

	}

	void initialize()
	{
		//Init Walls
		for each(Object* o in m_walls)
		{
			o->initialize();
		}
		//Init candies
		for each(Object* o in m_candies)
		{
			o->initialize();
		}
		//Init cherries
		for each(Object* o in m_cherries)
		{
			o->initialize();
		}
		//Init powerUps
		for each(Object* o in m_powerUps)
		{
			o->initialize();
		}
	}

	void update(float p_deltaTime)
	{
		////Init Walls
		//for each(Object* o in m_walls)
		//{
		//	o->update(p_deltaTime);
		//}
		////Init candies
		//for each(Object* o in m_candies)
		//{
		//	o->update(p_deltaTime);
		//}
		//Init cherries
		for each(Object* o in m_cherries)
		{
			o->update(p_deltaTime);
		}
		//Init powerUps
		for each(Object* o in m_powerUps)
		{
			o->update(p_deltaTime);
		}
		//Init enemies
		for each(Object* o in m_enemies)
		{
			o->update(p_deltaTime);
		}
		//Init Pacman
		m_pacman->update(p_deltaTime);
	}
};

#endif