#include "Object.h"

Object::Object(ID3D10Device* p_device, D3DXVECTOR3 p_position)
{
	m_device = p_device;
	m_position = p_position;

	D3DXMatrixIdentity(&m_rotation);
	D3DXMatrixIdentity(&m_scale);
	D3DXMatrixTranslation(&m_translate, m_position.x, m_position.y, m_position.z);
}

void Object::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	D3DXMATRIX worldViewProj = getWorldMatrix() * p_view * p_projection;

	/*fx::SpiderFX->SetMatrix("gWVP", worldViewProj);
	fx::SpiderFX->SetMatrix("gWorld", getWorldMatrix());
	fx::SpiderFX->SetMatrix("gNormalM", getNormalMatrix());
	*/

	/*D3D10_TECHNIQUE_DESC techDesc;
	fx::TowerFX->GetTechnique()->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		fx::TowerFX->Apply(p);
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