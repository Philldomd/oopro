#include "Shader.h"

Shader::Shader()
{
	m_pEffect = NULL;
	m_pTechnique = NULL;
	m_pInputLayout = NULL;
}

Shader::~Shader()
{
	SAFE_RELEASE(m_pInputLayout);
	//SAFE_RELEASE(m_pEffect);

	if(m_pEffect)
	{
		while(m_pEffect->Release());
		m_pEffect = NULL;
	}
}
HRESULT Shader::init(ID3D10Device* p_device, char* p_filename, DWORD p_shaderFlags)
{
	m_device = p_device;

	HRESULT hr = S_OK;

	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	ID3D10Blob* compilationErrors = 0;

	//DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#if defined(DEBUG) || defined(_DEBUG)
	//dwShaderFlags |= D3D10_SHADER_DEBUG;
	p_shaderFlags |= D3D10_SHADER_DEBUG;
	//shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	if(D3DX10CreateEffectFromFile(	p_filename,
		NULL,
		NULL,
		"fx_4_0",
		p_shaderFlags,
		0,
		m_device,
		NULL,
		NULL,
		&m_pEffect,
		&compilationErrors,
		NULL))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}
		//DXTrace(__FILE__, (DWORD)__LINE__, hr, "D3DX10CreateEffectFromFile", true);
	}

	return hr;
}

HRESULT Shader::init(ID3D10Device* p_device, char* p_filename,
		const D3D10_INPUT_ELEMENT_DESC* p_inputElementDesc, 
		unsigned int p_numElements, char* p_tecName, DWORD p_shaderFlags)
{
	m_device = p_device;

	HRESULT hr = S_OK;

	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	ID3D10Blob* compilationErrors = 0;
	
	//DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#if defined(DEBUG) || defined(_DEBUG)
	//dwShaderFlags |= D3D10_SHADER_DEBUG;
	p_shaderFlags |= D3D10_SHADER_DEBUG;
	//shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	
	if(FAILED(D3DX10CreateEffectFromFile(	p_filename,
											NULL,
											NULL,
											"fx_4_0",
											p_shaderFlags,
											0,
											m_device,
											NULL,
											NULL,
											&m_pEffect,
											&compilationErrors,
											NULL)))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}
		//DXTrace(__FILE__, (DWORD)__LINE__, hr, "D3DX10CreateEffectFromFile", true);
	}

	m_pTechnique = m_pEffect->GetTechniqueByName(p_tecName);


	if(p_inputElementDesc)
	{
		D3D10_PASS_DESC PassDesc;
		m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		if(FAILED(hr = m_device->CreateInputLayout(
			p_inputElementDesc,
			p_numElements,
			PassDesc.pIAInputSignature,
			PassDesc.IAInputSignatureSize,
			&m_pInputLayout
			)))
		{
			MessageBox(0, "Cannot create input layout.", "CreateInputLayout error", MB_OK | MB_ICONERROR);
			return hr;
		}
	}

	return hr;
}

HRESULT Shader::apply(unsigned int p_pass)
{
	ID3D10EffectPass* p = m_pTechnique->GetPassByIndex(p_pass);
	if(p)
	{
		 m_pTechnique->GetPassByIndex(p_pass)->Apply(0);

		if(m_pInputLayout)
		{
			m_device->IASetInputLayout(m_pInputLayout);
		}
		return S_OK;
	}

	return E_FAIL;
}

void Shader::setFloat(char* p_variable, float p_value)
{
	m_pEffect->GetVariableByName(p_variable)->AsScalar()->SetFloat(p_value);
}

void Shader::setMatrix(char* p_variable, D3DXMATRIX& p_mat)
{
	m_pEffect->GetVariableByName(p_variable)->AsMatrix()->SetMatrix((float*)&p_mat);
}

void Shader::setFloat3(char* p_variable, D3DXVECTOR3 p_value)
{
	m_pEffect->GetVariableByName(p_variable)->AsVector()->SetFloatVector((float*)&p_value);
}

void Shader::setFloat4(char* p_variable, D3DXVECTOR4 p_value)
{
	m_pEffect->GetVariableByName(p_variable)->AsVector()->SetFloatVector((float*)&p_value);
}

void Shader::setResource(char* p_variable, ID3D10ShaderResourceView* p_value)
{
	m_pEffect->GetVariableByName(p_variable)->AsShaderResource()->SetResource(p_value);
}

void Shader::setInt(char* p_variable, int p_value)
{
	m_pEffect->GetVariableByName(p_variable)->AsScalar()->SetInt(p_value);
}

void Shader::setBool(char* p_variable, bool p_value)
{
	m_pEffect->GetVariableByName(p_variable)->AsScalar()->SetBool(p_value);
}

void Shader::setRawData(char* p_variable, void* p_data, size_t p_size)
{
	m_pEffect->GetVariableByName(p_variable)->SetRawValue(p_data, 0, (UINT)p_size);
}

ID3D10EffectTechnique* Shader::getTechnique()
{
	return m_pTechnique;
}

ID3D10EffectTechnique* Shader::getTechniqueByName(char* p_tecName)
{
	return m_pEffect->GetTechniqueByName(p_tecName);
}

void Shader::setTechniqueByName(char* p_tecName)
{
	m_pTechnique = m_pEffect->GetTechniqueByName(p_tecName);
}