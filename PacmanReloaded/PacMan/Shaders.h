#ifndef SHADERS_
#define SHADERS_

#include "stdafx.h"
#include "Shader.h"
#include <vector>

using namespace std;

class Shaders
{
public:
	Shaders();
	~Shaders();

	void		init(ID3D10Device* p_d3dDevice);
	void		addShader(char* p_FXFileName, int p_shaderFlags);
	Shader*		getShaderByName(string p_FXFileName);

private:

	

private:

	ID3D10Device* m_d3dDevice;
	vector<Shader*> m_shaderList;
};
#endif