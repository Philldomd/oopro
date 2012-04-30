#ifndef WORLDLOADER_H
#define WORLDLOADER_H
#include <string>
#include "d3dApp.h"
#include "Factory.h"
#include "Buffer.h"
#include "GenerallStructs.h"
#include "ModelManager.h"

using namespace std;

//struct Objects
//{
//	vector<Object*> m_walls;
//	Object*			m_pacman;
//	vector<Object*> m_candies;
//	vector<Object*> m_enemies;
//	vector<Object*> m_cherries;
//	vector<Object*> m_powerUps;
//};

class WorldLoader
{
public:
	WorldLoader(ID3D10Device* p_device, ModelManager* p_modelManager);
	virtual ~WorldLoader();
	void			loadFromFile(string p_filename, UINT p_terrainWidth, 
								UINT p_terrainHeight, UINT p_terrainY,
								Objects& p_objects);

	D3DXVECTOR2*	getTerrainScale();	
	int				getNrWalls();
private:
	struct Color
	{
		int r,g,b,a;
		Color() 
			: r(0), g(0), b(0), a(0) { }
		Color(int pR, int pG, int pB, int pA)
			: r(pR), g(pG), b(pB), a(pA) { }

		bool operator==(const Color& c) const
		{
			if (r == c.r && g == c.g && b == c.b && a == c.a)
			{
				return true;
			} 
			else{ return false; }
		}
	};

	void findCorners();
	UINT nrOfNeighborsWithSameColor(int p_x, int p_y, Color p_color);
	bool isCorner(int p_x, int p_y, Color p_color);
	void checkColorRules(float p_x, float p_y, Color* p_color, Objects& p_objects);

	ID3D10Device*	m_device;
	ModelManager*	m_modelManager;
	UINT			m_height, m_width;
	UINT			m_terrainHeight, m_terrainWidth, m_terrainY;
	D3DXVECTOR2		m_terrainScale;
	vector<Color>	m_fileColorData;
	//vector<D3DXVECTOR2> m_corners; //Behövs troligen inte!
	//Matrix representation of the map
	vector<vector<char>> m_mapMatrix;

	//Factories
	WallFactory*	m_wallFactory;
	PacmanFactory*	m_pacmanFactory;
	EnemyFactory*	m_enemyFactory;
	CandyFactory*	m_candyFactory;
	PowerUpFactory* m_powerUpFactory;
	CherryFactory*	m_cherryFactory;

	//DEBUG Skall tas bort!!
	UINT nrCandy, nrCherry, nrPowerUp, nrEnemies, nrPlayer, nrWalls;
};

#endif