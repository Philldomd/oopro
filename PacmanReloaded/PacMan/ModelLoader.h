#ifndef MODELLOADER_
#define MODELLOADER_

#include "ModelStructs.h"
#include "VertexStructs.h"
#include "Buffer.h"
#include "stdafx.h"

#include <fstream>
#include <sstream>

#include <string>
#include <vector>


using namespace std;

class ModelLoader
{

public:
	
	ModelLoader(ID3D10Device* p_d3dDevice);
	AnimatedModel			addAnimatedModel(string p_modelName, string p_OBJFileName);
	Model					addStaticModel(string p_modelName, string p_OBJFileName);
	~ModelLoader();

private:

	HRESULT		createBuffers();
	void		calculateBoundingBox(D3DXVECTOR3 p_vector3);
	void		loadMaterialFromMTL(string p_materialFileName);
	void		loadModelFromOBJFile(string p_OBJFileName);
	void		loadAnimatedModelFromOBJFile(string p_OBJFileName);
	int			calculateIndex(Vertex* p_vertex);
	ID3D10ShaderResourceView*		createTexture(string p_textureFileName);

private:

	ID3D10Device*		m_d3dDevice;

	vector<Vertex>		m_vertices;
	vector<int>			m_indices;

	Buffer*				m_vertexBuffer;
	Buffer*				m_indexBuffer;

	string				m_resourceFolder;
	string				m_OBJFileName;

	vector<D3DXVECTOR3>	m_positions;
	vector<D3DXVECTOR2> m_texCoords;
	vector<D3DXVECTOR3> m_normals;

	Material*			m_material;
	Model				m_model;
	AnimatedModel		m_animatedModel;

	D3DXVECTOR3			m_topCorner;
	D3DXVECTOR3			m_bottomCorner;

	int					m_nrOfStances;
	int					m_stanceSize;
};
#endif