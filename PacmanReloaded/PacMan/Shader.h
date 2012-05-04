#ifndef SHADER__H
#define SHADER__H

#include "stdafx.h"
#include <DxErr.h>

class Shader
{

	ID3D10Device*			m_device;
	ID3D10InputLayout*		m_pInputLayout;

	ID3D10Effect*			m_pEffect;
	ID3D10EffectTechnique*	m_pTechnique;

public:
	Shader();
	~Shader();
	HRESULT					init(ID3D10Device* p_device, char* p_filename, DWORD p_shaderFlags);
	HRESULT					init(ID3D10Device* p_device, char* p_filename, 
								const D3D10_INPUT_ELEMENT_DESC* p_inputElementDesc, 
								unsigned int p_numElements,char* p_tecName, DWORD p_shaderFlags );

	HRESULT					apply(unsigned int p_pass);

	void					setMatrix(char* p_variable, D3DXMATRIX& p_mat);
	void					setFloat(char* p_variable, float p_value);
	void					setFloat3(char* p_variable, D3DXVECTOR3 p_value);
	void					setFloat4(char* p_variable, D3DXVECTOR4 p_value);

	void					setInt(char* p_variable, int p_value);
	void					setBool(char* p_variable, bool p_value);
	void					setRawData(char* p_variable, void* p_data, size_t p_size);

	void					setResource(char* p_variable, ID3D10ShaderResourceView* p_value);

	ID3D10EffectTechnique*	getTechnique();
	ID3D10EffectTechnique*	getTechniqueByName(char* p_tecName);
	void					setTechniqueByName(char* p_tecName);
};
#endif