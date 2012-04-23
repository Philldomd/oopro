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
HRESULT Shader::Init(ID3D10Device* device, char* filename, DWORD shaderFlags)
{
	mDevice = device;

	HRESULT hr = S_OK;

	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	ID3D10Blob* compilationErrors = 0;

	//DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#if defined(DEBUG) || defined(_DEBUG)
	//dwShaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_DEBUG;
	//shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	if(D3DX10CreateEffectFromFile(	filename,
		NULL,
		NULL,
		"fx_4_0",
		shaderFlags,
		0,
		mDevice,
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

HRESULT Shader::Init(ID3D10Device* device, char* filename,
		const D3D10_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int numElements, char* tecName, DWORD shaderFlags)
{
	mDevice = device;

	HRESULT hr = S_OK;

	ID3DBlob*	pBlobEffect = NULL;
	ID3DBlob*	pBlobErrors = NULL;
	ID3D10Blob* compilationErrors = 0;
	
	//DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;

#if defined(DEBUG) || defined(_DEBUG)
	//dwShaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_DEBUG;
	//shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	
	if(FAILED(D3DX10CreateEffectFromFile(	filename,
											NULL,
											NULL,
											"fx_4_0",
											shaderFlags,
											0,
											mDevice,
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

	m_pTechnique = m_pEffect->GetTechniqueByName(tecName);


	if(inputElementDesc)
	{
		D3D10_PASS_DESC PassDesc;
		m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		if(FAILED(hr = mDevice->CreateInputLayout(
			inputElementDesc,
			numElements,
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

HRESULT Shader::Apply(unsigned int pass)
{
	ID3D10EffectPass* p = m_pTechnique->GetPassByIndex(pass);
	if(p)
	{
		 m_pTechnique->GetPassByIndex(pass)->Apply(0);

		if(m_pInputLayout)
		{
			mDevice->IASetInputLayout(m_pInputLayout);
		}
		return S_OK;
	}

	return E_FAIL;
}

void Shader::SetFloat(char* variable, float value)
{
	m_pEffect->GetVariableByName(variable)->AsScalar()->SetFloat(value);
}

void Shader::SetMatrix(char* variable, D3DXMATRIX& mat)
{
	m_pEffect->GetVariableByName(variable)->AsMatrix()->SetMatrix((float*)&mat);
}

void Shader::SetFloat3(char* variable, D3DXVECTOR3 value)
{
	m_pEffect->GetVariableByName(variable)->AsVector()->SetFloatVector((float*)&value);
}

void Shader::SetFloat4(char* variable, D3DXVECTOR4 value)
{
	m_pEffect->GetVariableByName(variable)->AsVector()->SetFloatVector((float*)&value);
}

void Shader::SetResource(char* variable, ID3D10ShaderResourceView* value)
{
	m_pEffect->GetVariableByName(variable)->AsShaderResource()->SetResource(value);
}

//void Shader::SetUnorderedAccessView(char* variable, ID3D10UnorderedAccessView* value)
//{
//	m_pEffect->GetVariableByName(variable)->AsUnorderedAccessView()->SetUnorderedAccessView(value);
//}

void Shader::SetInt(char* variable, int value)
{
	m_pEffect->GetVariableByName(variable)->AsScalar()->SetInt(value);
}

void Shader::SetBool(char* variable, bool value)
{
	m_pEffect->GetVariableByName(variable)->AsScalar()->SetBool(value);
}

void Shader::SetRawData(char* variable, void* data, size_t size)
{
	m_pEffect->GetVariableByName(variable)->SetRawValue(data, 0, (UINT)size);
}

ID3D10EffectTechnique* Shader::GetTechnique()
{
		return m_pTechnique;
}

ID3D10EffectTechnique* Shader::GetTechniqueByName(char* tecName)
{
	return m_pEffect->GetTechniqueByName(tecName);
}

void Shader::SetTechniqueByName(char* tecName)
{
	m_pTechnique = m_pEffect->GetTechniqueByName(tecName);
}