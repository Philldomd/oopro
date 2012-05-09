#include "ModelManager.h"

ModelManager::ModelManager(ID3D10Device* p_d3dDevice)
{
	m_d3dDevice = p_d3dDevice;
}

Model* ModelManager::getModelByName( string p_modelName )
{
	for (int i = 0; i < (int)m_modelList.size(); i++)
	{
		if(m_modelList[i].m_bufferName == p_modelName)
		{
			return &m_modelList[i];
		}

	}
	return &m_modelList[0];
}

void ModelManager::createModel( string p_modelName, string p_OBJFileName )
{
	ModelLoader* t_modelLoader = new ModelLoader(m_d3dDevice);

	m_modelList.push_back(t_modelLoader->addStaticModel(p_modelName, p_OBJFileName));

	delete t_modelLoader;
}



