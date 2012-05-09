#ifndef INSTANCING_H
#define INSTANCING_H
#include "D3dx9math.h"
#include <D3D10.h>
#include "Object.h"
#include "Shader.h"
#include <vector>
#include "Buffer.h"

using namespace std;

class Instancing
{
public:
	Instancing(ID3D10Device* p_device);
	~Instancing();
	void	initializeDefault(vector<Object*>* p_objects, Shader* p_shader);
	void	initializeDynamic(vector<Object*>* p_objects, Shader* p_shader);
	void	updateDefault(float p_gameTime);
	void	updateDynamic(float p_gameTime);
	void	render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection);

	//TEMP
	void temp(vector<Object*>* p_objects);
private:
	ID3D10Device*		m_device;
	vector<Object*>*	m_objects;

	Buffer*				mInstanceData;

	ID3D10Buffer*		m_VB[2];
	UINT				m_strides[2];
	D3DXMATRIX			m_rotation;

	UINT				m_previousObjectSize;
	float				m_angle;

	Shader*				m_shader;
};
#endif