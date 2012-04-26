#ifndef TERRAIN__H
#define TERRAIN__H

#include "stdafx.h"
#include "Buffer.h"
#include "Effects.h"
#include "WorldLoader.h"
#include "OBJLoader.h"
#include "Mesh.h"
//#include "HeightMap.h"
//#include "DrawTex2D.h"
//#include "GameTimer.h" //DEBUG
//#include "Lights.h"

class Terrain
{
public:
	Terrain(ID3D10Device* pDevice, int pHeight, int pWidth);
	~Terrain();
	HRESULT init();
	void prepToRender(D3DXMATRIX& pWorld, D3DXVECTOR3 pEyePos);
	void render(D3DXMATRIX& pView, D3DXMATRIX& pProjection);

private:
	struct INSTANCEDATA
	{
		D3DXMATRIX mMatWorld;
	};
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texCoord;
		D3DXVECTOR3 normal;
	};
	void createIndices(int** pIndices);
	void createVertices(Vertex** pVertices);

	//DrawTex2D* mShadowTex;
	Buffer* mIndexBuffer;
	Buffer* mVertexBuffer;

	ID3D10Device* mDevice;

	float mTextureRepeat;
	float mTextureScale;

	int mHeight;
	int mNrOfIndices;
	int mNrOfVertices;
	int mWidth;
	//vector<Object*> m_objects;
	WorldLoader* mLoader;
	OBJLoader* objLoader;
	Mesh* m_mesh;
	Buffer* mInstanceData;
	int		mWallIndex;

	

	Objects			m_objects;

	//DEBUG
	float angle;
	D3DXMATRIX	m_rotation;
};
#endif