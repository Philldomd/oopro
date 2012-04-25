#ifndef MODELLOADER_
#define MODELLOADER_

#include "ModelStructs.h"
#include "VertexStructs.h"
#include "Buffer.h"
#include <fstream>
#include <sstream>

using namespace std;

class ModelLoader
{

public:
	
	ModelLoader();
	AnimatedModel			addAnimatedModel(string p_modelName, string p_OBJFileName);
	Model					addStaticModel(string p_modelName, string p_OBJFileName);
	~ModelLoader();

private:

	void		createBuffers();
	void		calculateBoundingBox();
	void		loadMaterialFromMTL(string p_materialFileName);
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
	vector<D3DXVECTOR3> m_texCoords;
	vector<D3DXVECTOR3> m_normals;
	Material			m_material;
	Model				m_model;
	AnimatedModel		m_animatedModel;

};
#endif