#ifndef SHADER__H
#define SHADER__H

#include "stdafx.h"
#include <DxErr.h>

class Shader
{

	ID3D10Device*			mDevice;
	ID3D10InputLayout*		m_pInputLayout;

	ID3D10Effect*			m_pEffect;
	ID3D10EffectTechnique*	m_pTechnique;

public:
	Shader();
	~Shader();
	HRESULT Init(ID3D10Device* device, char* filename, DWORD shaderFlags);
	HRESULT Init(ID3D10Device* device, char* filename, 
		const D3D10_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements,char* tecName, DWORD shaderFlags );

	HRESULT Apply(unsigned int pass);

	void SetMatrix(char* variable, D3DXMATRIX& mat);
	void SetFloat(char* variable, float value);
	void SetFloat3(char* variable, D3DXVECTOR3 value);
	void SetFloat4(char* variable, D3DXVECTOR4 value);

	void SetInt(char* variable, int value);
	void SetBool(char* variable, bool value);
	void SetRawData(char* variable, void* data, size_t size);

	void SetResource(char* variable, ID3D10ShaderResourceView* value);

	ID3D10EffectTechnique* GetTechnique();
	ID3D10EffectTechnique* GetTechniqueByName(char* tecName);
	void SetTechniqueByName(char* tecName);
	
	//void SetUnorderedAccessView(char* variable, ID3D11UnorderedAccessView* value);
};

#endif
