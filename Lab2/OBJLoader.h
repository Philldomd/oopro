#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "stdafx.h"
#include "Mesh.h"
#include <sstream>

using namespace std;

class OBJLoader
{
private:
	vector<D3DXVECTOR3> m_positionArray;
	vector<D3DXVECTOR2> m_texCoordArray;
	vector<D3DXVECTOR3> m_normalArray;
void Clear();
void ShrinkToFit();

public:
	OBJLoader();
	~OBJLoader();

HRESULT LoadOBJ(char* p_filename, Mesh* p_mesh);
};
#endif