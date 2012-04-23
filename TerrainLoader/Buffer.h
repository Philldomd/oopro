#ifndef BUFFER__H
#define BUFFER__H

#include "stdafx.h"

enum BUFFER_TYPE
{
	VERTEX_BUFFER,
	INDEX_BUFFER,
	CONSTANT_BUFFER_VS,
	CONSTANT_BUFFER_GS,
	CONSTANT_BUFFER_PS,
	BUFFER_TYPE_COUNT
};

enum BUFFER_USAGE
{
	BUFFER_DEFAULT,
	BUFFER_STREAM_OUT_TARGET,
	BUFFER_CPU_WRITE,
	BUFFER_CPU_WRITE_DISCARD,
	BUFFER_CPU_READ,
	BUFFER_USAGE_COUNT
};

struct BUFFER_INIT_DESC
{
	BUFFER_TYPE		Type;
	UINT32			NumElements;
	UINT32			ElementSize;
	BUFFER_USAGE	Usage;
	void*			InitData;

	BUFFER_INIT_DESC()
	{
		InitData = NULL;
	}
};

class Buffer
{
protected:
	ID3D10Buffer*	mBuffer;
	BUFFER_TYPE		mType;
	BUFFER_USAGE	mUsage;

	UINT32			mElementSize;
	UINT32			mElementCount;
	UINT32			vertexSize;
	UINT32			offset;

	ID3D10Device*			mDevice;
public:
	Buffer();
	virtual ~Buffer();

	HRESULT Init(ID3D10Device* device,
		BUFFER_INIT_DESC& initDesc);

	void Map(void** b);
	void Unmap();

	HRESULT Apply(UINT32 misc = 0);

	ID3D10Buffer* GetBufferPointer();
	void SetBufferPointer(ID3D10Buffer* p);
	UINT32 GetVertexSize();
	UINT32 GetElementCount();
	UINT32* GetOffset();
};

#endif
