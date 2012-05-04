#ifndef CANDYINSTANCING_H
#define CANDYINSTANCING_H
#include "D3dx9math.h"
#include <D3D10.h>
#include "Object.h"
#include "Effects.h"
#include <vector>
#include "Buffer.h"

using namespace std;

class CandyInstancing
{
public:
	CandyInstancing(ID3D10Device* p_device);
	~CandyInstancing();
	void	initialize(vector<Object*>* p_objects);
	void	update(float p_deltaTime);
	void	render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection);
private:
	ID3D10Device*		m_device;
	vector<Object*>*	m_objects;

	Buffer*				mInstanceData;

	ID3D10Buffer*		m_VB[2];
	UINT				m_strides[2];

	UINT				m_previousObjectSize;

	D3DXMATRIX			m_rotation;
	float				m_angle;
};
#endif