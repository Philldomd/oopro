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

	ModelManager();
	Model*			getModelByName(string p_modelName);
	AnimatedModel*	getAnimatedModelByName(string p_animatedModelName);
	void			createModel(string p_modelName, string p_OBJFileName);
	void			createAnimatedModel(string p_animatedModelName, string p_OBJFileName);
	~ModelManager();

private:

	vector<Model>			m_modelList;
	vector<AnimatedModel>	m_animatedModelList;

};
#endif
