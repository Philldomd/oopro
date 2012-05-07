#include "Object.h"

Object::Object(ID3D10Device* p_device, D3DXVECTOR3 p_position)
{
	m_device = p_device;
	m_position = p_position;

	//m_model = p_modelManager.getbyname("");
	//m_shader = p_shaderManager.getbyname("");

	D3DXMatrixIdentity(&m_rotation);
	D3DXMatrixIdentity(&m_scale);
	D3DXMatrixTranslation(&m_translate, m_position.x, m_position.y, m_position.z);
}

void Object::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	D3DXMATRIX worldViewProj = getWorldMatrix() * p_view * p_projection;
	/*
	m_model->setVertexBuffer()
	m_model->setIndexBuffer()
	
	m_shader->SetMatrix("gWVP", worldViewProj);
	m_shader->SetMatrix("gWorld", getWorldMatrix());
	m_shader->SetMatrix("gNormalM", getNormalMatrix());
	*/

	/*D3D10_TECHNIQUE_DESC techDesc;
	m_shader->GetTechnique()->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		m_shader->Apply(p);
		mDevice->DrawIndexed( mModel->size,0, 0);
	}*/
}

D3DXVECTOR3 Object::getPosition()
{
	return m_position;
}

D3DXMATRIX Object::getWorldMatrix()
{
	return m_rotation * m_scale * m_translate;
}

D3DXMATRIX Object::getNormalMatrix()
{
	D3DXMATRIX normalMatrix, inversMatrix;
	D3DXMatrixInverse(&inversMatrix, NULL, &getWorldMatrix());
	D3DXMatrixTranspose(&normalMatrix, &inversMatrix);

	return normalMatrix;
}

Model* Object::getModel()
{
	return m_model;
}

void Object::createBoundingVolume()
{
	m_volume = new AABB( m_model->m_topBoundingCorner, m_model->m_bottomBoundingCorner, D3DXVECTOR4(1,1,1,1), m_device );
}

BoundingVolume* Object::getBoundingVolume()
{
	return m_volume;
}