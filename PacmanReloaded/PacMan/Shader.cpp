#include "Shader.h"

Shader::Shader()
{
	m_pEffect = NULL;
	m_pTechnique = NULL;
	m_inputLayout = NULL;
}

Shader::~Shader()
{
	SAFE_RELEASE(m_inputLayout);
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

	m_FXFileName =  p_filename;

	HRESULT hr = S_OK;

	ID3D10Blob*	pBlobEffect = NULL;
	ID3D10Blob* compilationErrors = 0;

	if(D3DX10CompileFromFile( p_filename, NULL, NULL, "VSInstmain", "vs_4_0", p_shaderFlags, NULL, NULL, &pBlobEffect, &compilationErrors, NULL ))
	{
		if ( compilationErrors != NULL )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}
	}
	if(pBlobEffect)
		createInputLayoutDescFromVertexShaderSignature(pBlobEffect);
	else
		return S_FALSE;

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

	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	return hr;
}

void Shader::createInputLayoutDescFromVertexShaderSignature(ID3D10Blob* p_shaderBlob)
{
	m_inputLayout = NULL;
	bool firstInstanceLayout = true;
	// Reflect shader info
	ID3D10ShaderReflection* pVertexShaderReflection = NULL;	
	D3D10ReflectShader( p_shaderBlob->GetBufferPointer(), p_shaderBlob->GetBufferSize(), &pVertexShaderReflection);


	// Get shader info
	D3D10_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc( &shaderDesc );

	// Read input layout description from shader info
	UINT32 byteOffset = 0;
	std::vector<D3D10_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT32 i=0; i< shaderDesc.InputParameters; i++)
	{
		D3D10_SIGNATURE_PARAMETER_DESC paramDesc;		
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc );

		// fill out input element desc
		D3D10_INPUT_ELEMENT_DESC elementDesc;	
		elementDesc.SemanticName = paramDesc.SemanticName;		
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		//elementDesc.InstanceDataStepRate = 0;
		//elementDesc.InputSlot = 0;

		string tempSemanticName = paramDesc.SemanticName;

		if(tempSemanticName == "mTransform")
		{
			if (firstInstanceLayout)
			{
				byteOffset = 0;
				firstInstanceLayout = false;
			}
			elementDesc.InputSlot = 1;
			elementDesc.InputSlotClass = D3D10_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}
		else
		{
			elementDesc.InputSlot = 0;
			elementDesc.InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
		}

		elementDesc.AlignedByteOffset = byteOffset;

		// determine DXGI format
		if ( paramDesc.Mask == 1 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			byteOffset += 4;
		}
		else if ( paramDesc.Mask <= 3 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			byteOffset += 8;
		}
		else if ( paramDesc.Mask <= 7 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
		}
		else if ( paramDesc.Mask <= 15 )
		{
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}		

	// Try to create Input Layout
	HRESULT hr = m_device->CreateInputLayout( &inputLayoutDesc[0], inputLayoutDesc.size(), p_shaderBlob->GetBufferPointer(), p_shaderBlob->GetBufferSize(), &m_inputLayout );

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();

}


HRESULT Shader::apply(unsigned int p_pass)
{
	ID3D10EffectPass* p = m_pTechnique->GetPassByIndex(p_pass);
	if(p)
	{
		m_pTechnique->GetPassByIndex(p_pass)->Apply(0);

		if(m_inputLayout)
		{
			m_device->IASetInputLayout(m_inputLayout);
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

string Shader::getFXFileName()
{
	return m_FXFileName;
}
