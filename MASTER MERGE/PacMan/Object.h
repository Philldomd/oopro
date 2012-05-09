#ifndef OBJECT_H
#define OBJECT_H
#include "D3dx9math.h"
#include <D3D10.h>
#include "ModelManager.h"
#include "BoundingVolume.h"
#include "AABB.h"

class Object
{
public:
	Object(ID3D10Device* p_device, D3DXVECTOR3 p_position);
	virtual ~Object();
	virtual void	initialize();
	virtual void	update(float p_deltaTime);
	virtual void	render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection);
	Model*			getModel();
	D3DXVECTOR3		getPosition();
	D3DXMATRIX		getWorldMatrix();
	D3DXMATRIX		getRotationMatrix();
	D3DXMATRIX		getNormalMatrix(); //Returns a matrix for transforming normals
	AABB*			getBoundingBox();
	//Model*		getModel();
protected:
	void			createBoundingVolume();
	
	ID3D10Device*	m_device;

	D3DXVECTOR3		m_position;
	D3DXMATRIX		m_rotation;
	D3DXMATRIX		m_translate;
	D3DXMATRIX		m_scale;
	D3DXMATRIX		m_normalMatrix; //Used to transform normals
	Model* m_model;
	//Shader* m_shader;
	AABB* m_volume;
};
#endif