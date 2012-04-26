#include "Buffer.h"

Buffer::Buffer()
{
	mBuffer = NULL;
	vertexSize = 0;
	offset = 0;
}

Buffer::~Buffer()
{
	SAFE_RELEASE(mBuffer);
}

HRESULT Buffer::Apply(UINT32 misc)
{
	HRESULT hr = S_OK;

	switch(mType)
	{
	case VERTEX_BUFFER:
		{
			vertexSize = mElementSize;
			offset = 0;
			mDevice->IASetVertexBuffers(misc, 1, &mBuffer, &vertexSize, &offset );
		}
		break;
	case INDEX_BUFFER:
		{
			mDevice->IASetIndexBuffer(mBuffer, DXGI_FORMAT_R32_UINT, 0);
		}
		break;
	case CONSTANT_BUFFER_VS:
		{
			mDevice->VSSetConstantBuffers(misc, 1, &mBuffer);
		}
		break;
	case CONSTANT_BUFFER_GS:
		{
			mDevice->GSSetConstantBuffers(misc, 1, &mBuffer);
		}
		break;
	case CONSTANT_BUFFER_PS:
		{
			mDevice->PSSetConstantBuffers(misc, 1, &mBuffer);
		}
		break;
	default:
		hr = E_FAIL;
		break;
	};

	return hr;
}

HRESULT Buffer::Init(ID3D10Device* device,
		BUFFER_INIT_DESC& initDesc)
{
	mDevice = device;
	
	D3D10_BUFFER_DESC bufferDesc;

	mType = initDesc.Type;
	switch(mType)
	{
		case VERTEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;

				if(initDesc.Usage == BUFFER_STREAM_OUT_TARGET)
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

	mUsage = initDesc.Usage;
	mElementSize = initDesc.ElementSize;
	mElementCount = initDesc.NumElements;

	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;

	if(mUsage == BUFFER_CPU_READ)
	{
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D10_CPU_ACCESS_READ;
	}
	else if(mUsage == BUFFER_CPU_WRITE)
	{
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D10_CPU_ACCESS_WRITE;
	}
	else if(mUsage == BUFFER_CPU_WRITE_DISCARD)
	{
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D10_CPU_ACCESS_WRITE;
	}

	//Desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = initDesc.NumElements * initDesc.ElementSize;

	//set at least 16 bytes
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	HRESULT hr = S_OK;
	if(initDesc.InitData)
	{
		D3D10_SUBRESOURCE_DATA data;
		data.pSysMem = initDesc.InitData;
		hr = mDevice->CreateBuffer(&bufferDesc, &data, &mBuffer);
	}
	else
	{
		hr = mDevice->CreateBuffer(&bufferDesc, NULL, &mBuffer);
	}

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create buffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return hr;
}

void Buffer::Map(void** b)
{
	UINT32 mapType = 0;

	if(mUsage == BUFFER_CPU_READ)	mapType = D3D10_MAP_READ;
	else if(mUsage == BUFFER_CPU_WRITE) mapType = D3D10_MAP_WRITE;
	else if(mUsage == BUFFER_CPU_WRITE_DISCARD)	mapType = D3D10_MAP_WRITE_DISCARD;
	else
	{
		mapType = D3D10_MAP_WRITE_DISCARD;
	}

	//mBuffer->Map( (D3D10_MAP)mapType, 0, reinterpret_cast< void** >(b) );
	mBuffer->Map( (D3D10_MAP)mapType, 0, (LPVOID*)b );
}

void Buffer::Unmap()
{
	mBuffer->Unmap();
}

ID3D10Buffer* Buffer::GetBufferPointer()
{
	return mBuffer;
}

UINT32 Buffer::GetVertexSize()
{
	return mElementSize;
}

UINT32 Buffer::GetElementCount()
{
	return mElementCount;
}

UINT32* Buffer::GetOffset()
{
	return &offset;
}

void Buffer::SetBufferPointer(ID3D10Buffer* p)
{
	mBuffer = p;
}