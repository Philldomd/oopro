#ifndef MODELMANAGER_
#define MODELMANAGER_

#include "ModelStructs.h"
#include "ModelLoader.h"
#include <vector>
#include <string>


using namespace std;

class ModelManager
{
public:

	ModelManager(ID3D10Device* p_d3dDevice);
	Model*			getModelByName(string p_modelName);
	void			createModel(string p_modelName, string p_OBJFileName);
	~ModelManager();

private:

	vector<Model>			m_modelList;
	ID3D10Device*			m_d3dDevice;

};
#endif
