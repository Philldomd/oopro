#include "Buffer.h"

Buffer::Buffer()
{
	m_buffer = NULL;
	m_vertexSize = 0;
	m_offset = 0;
}

Buffer::~Buffer()
{
	SAFE_RELEASE(m_buffer);
}

HRESULT Buffer::apply(UINT32 p_misc)
{
	HRESULT hr = S_OK;

	switch(m_type)
	{
		case VERTEX_BUFFER:
			{
				m_vertexSize = m_elementSize;
				m_offset = 0;
				m_device->IASetVertexBuffers(p_misc, 1, &m_buffer, &m_vertexSize, &m_offset );
			}
			break;
		case INDEX_BUFFER:
			{
				m_device->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
			}
			break;
		case CONSTANT_BUFFER_VS:
			{
				m_device->VSSetConstantBuffers(p_misc, 1, &m_buffer);
			}
			break;
		case CONSTANT_BUFFER_GS:
			{
				m_device->GSSetConstantBuffers(p_misc, 1, &m_buffer);
			}
			break;
		case CONSTANT_BUFFER_PS:
			{
				m_device->PSSetConstantBuffers(p_misc, 1, &m_buffer);
			}
			break;
		default:
			hr = E_FAIL;
			break;
	};

	return hr;
}

HRESULT Buffer::init(ID3D10Device* p_device,
		BUFFER_INIT_DESC& p_initDesc)
{
	m_device = p_device;
	
	D3D10_BUFFER_DESC bufferDesc;

	m_type = p_initDesc.Type;
	switch(m_type)
	{
		case VERTEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;

				if(p_initDesc.Usage == BUFFER_STREAM_OUT_TARGET)
					bufferDesc.BindFlags |= D3D10_BIND_STREAM_OUTPUT;
			}
			break;
		case INDEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
			}
			break;
		case CONSTANT_BUFFER_VS:
		case CONSTANT_BUFFER_GS:
		case CONSTANT_BUFFER_PS:
			{
				bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
			}
			break;
		default:
			return E_FAIL;
			break;
	};

	m_usage = p_initDesc.Usage;
	m_elementSize = p_initDesc.ElementSize;
	m_elementCount = p_initDesc.NumElements;

	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;

	if(m_usage == BUFFER_CPU_READ)
	{
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D10_CPU_ACCESS_READ;
	}
	else if(m_usage == BUFFER_CPU_WRITE)
	{
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D10_CPU_ACCESS_WRITE;
	}
	else if(m_usage == BUFFER_CPU_WRITE_DISCARD)
	{
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D10_CPU_ACCESS_WRITE;
	}

	//Desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = p_initDesc.NumElements * p_initDesc.ElementSize;

	//set at least 16 bytes
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	HRESULT hr = S_OK;
	if(p_initDesc.InitData)
	{
		D3D10_SUBRESOURCE_DATA data;
		data.pSysMem = p_initDesc.InitData;
		hr = m_device->CreateBuffer(&bufferDesc, &data, &m_buffer);
	}
	else
	{
		hr = m_device->CreateBuffer(&bufferDesc, NULL, &m_buffer);
	}

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create buffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return hr;
}

void Buffer::map(void** p_b)
{
	UINT32 mapType = 0;

	if(m_usage == BUFFER_CPU_READ)
		mapType = D3D10_MAP_READ;
	else if(m_usage == BUFFER_CPU_WRITE)
		mapType = D3D10_MAP_WRITE;
	else if(m_usage == BUFFER_CPU_WRITE_DISCARD)
		mapType = D3D10_MAP_WRITE_DISCARD;
	else
		mapType = D3D10_MAP_WRITE_DISCARD;

	m_buffer->Map( (D3D10_MAP)mapType, 0, (LPVOID*)p_b );
}

void Buffer::unMap()
{
	m_buffer->Unmap();
}

ID3D10Buffer* Buffer::getBufferPointer()
{
	return m_buffer;
}

UINT32 Buffer::getVertexSize()
{
	return m_elementSize;
}

UINT32 Buffer::getElementCount()
{
	return m_elementCount;
}

UINT32 Buffer::getOffset()
{
	return m_offset;
}

void Buffer::setBufferPointer(ID3D10Buffer* p_b)
{
	m_buffer = p_b;
}