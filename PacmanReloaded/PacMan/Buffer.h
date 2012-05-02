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
	ID3D10Buffer*	m_buffer;
	BUFFER_TYPE		m_type;
	BUFFER_USAGE	m_usage;

	UINT32			m_elementSize;
	UINT32			m_elementCount;
	UINT32			m_vertexSize;
	UINT32			m_offset;

	ID3D10Device*	m_device;
public:
	Buffer();
	virtual ~Buffer();

	HRESULT			init(	ID3D10Device* p_device,
							BUFFER_INIT_DESC& p_initDesc);

	void			map(void** p_b);
	void			unMap();

	HRESULT			apply(UINT32 p_misc = 0);

	ID3D10Buffer*	getBufferPointer();
	void			setBufferPointer(ID3D10Buffer* p_b);

	UINT32			getVertexSize();
	UINT32			getElementCount();
	UINT32			getOffset();
};
#endif