#include "Shaders.h"


Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}

void Shaders::init( ID3D10Device* p_d3dDevice )
{
	m_d3dDevice = p_d3dDevice;
}

void Shaders::addShader(char* p_FXFileName, int p_shaderFlags)
{
	m_shaderList.push_back(new Shader());
	m_shaderList.at(m_shaderList.size()-1)->init(m_d3dDevice, p_FXFileName, p_shaderFlags);
}

Shader* Shaders::getShaderByName(string p_FXFileName)
{
	for (int i = 0; i < (int)m_shaderList.size(); i++)
	{
		if(m_shaderList[i]->getFXFileName() == p_FXFileName)
		{
			return m_shaderList[i];
		}

	}
	return m_shaderList[0];
}


