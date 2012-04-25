#include "ModelManager.h"

ModelManager::ModelManager()
{

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

AnimatedModel* ModelManager::getAnimatedModelByName( string p_animatedModelName )
{
	for (int i = 0; i < (int)m_animatedModelList.size(); i++)
	{
		if(m_animatedModelList[i].m_bufferName == p_animatedModelName)
		{
			return &m_animatedModelList[i];
		}

	}
	return &m_animatedModelList[0];
}

void ModelManager::createModel( string p_modelName, string p_OBJFileName )
{

}

void ModelManager::createAnimatedModel( string p_animatedModelName, string p_OBJFileName )
{

}


