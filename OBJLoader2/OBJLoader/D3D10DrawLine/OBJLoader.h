#include "stdafx.h"
#include <fstream>
#include <sstream>
using namespace std;

struct Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texCoord;
	D3DXVECTOR3 normal; 
};
struct Material
{
	string name;
	int illum;
	D3DXVECTOR3 Kd;
	D3DXVECTOR3 Ka;
	D3DXVECTOR3 Tf;
	char map_Kd[MAX_PATH];
	float Ni;
};
class Loader
{
public:
	Loader();
	bool loadFile(string fileName);
	D3DXVECTOR3 GetVertexPos(int i);
	D3DXVECTOR3 GetVertexNorm(int i);
	D3DXVECTOR2 GetVertexTexCoord(int i);
	int GetVertBuffLength();
	void mtlLoader(string fileName);
	Material GetMaterialAt(int i);
private:
	vector<string> mtlName;
	string fileNameTex;
	Material loadMtl;
	vector<Material> mtl;
	Vertex vertex;
	vector<D3DXVECTOR3> Positions;
	vector<D3DXVECTOR2> TexCoords;
	vector<D3DXVECTOR3> Normals; 
	vector<Vertex> Vertices;
};