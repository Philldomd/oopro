#include "Enemy.h"

Enemy::Enemy(	ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, 
	D3DXVECTOR2 p_size,vector<vector<char>>* p_mapMatrix, Type p_type)
	:Object(p_device, p_position)
{
	m_mapMatrix = p_mapMatrix;
	m_model = p__model;
	m_speed = 10;
	m_speedScale = 1.0f;
	m_tileSize = p_size;
	float scaleMultiplier = 0.03f;
	D3DXMatrixScaling(&m_scale, scaleMultiplier * p_size.x, 
		scaleMultiplier * (p_size.y+p_size.x)/2, scaleMultiplier * p_size.y);
	m_type = p_type;
	m_ai = new BlinkyAI();
}

Enemy::~Enemy()
{

}

void Enemy::initialize(Shader* p_shader)
{
	m_shader = p_shader;

	////////////ONLY FOR TESTING/////////////
	float p_arrayScatter[] = {7,7,5,5};
	float p_arrayChase[] = {20,20,20,INT_MAX};
	m_ai->init(0, 20, 10, p_arrayScatter, p_arrayChase, 6, *m_mapMatrix, D3DXVECTOR2(25, 0), D3DXVECTOR2(14, 14));
	createBoundingVolume();
	m_volume->updatePosition( m_scale, m_translate );
}

void Enemy::update(D3DXVECTOR2 p_pacmanPos, D3DXVECTOR2 p_pacmanDirection, float gameTime)
{
	//Needs pacmans position and direction
	m_ai->update(p_pacmanPos, p_pacmanDirection, D3DXVECTOR2(m_position.x, m_position.y), gameTime);
	m_volume->updatePosition( m_scale, m_translate );
	m_direction = m_ai->update(p_pacmanPos, p_pacmanDirection, D3DXVECTOR2(m_position.x, m_position.y), gameTime);

	m_position.x += m_direction.x * m_speed * gameTime;
	m_position.z += m_direction.y * m_speed * gameTime;

	D3DXMatrixTranslation(&m_translate, m_position.x, m_position.y, m_position.z);
}

void Enemy::render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection)
{
	m_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMATRIX worldViewProj = getWorldMatrix() * p_view * p_projection;
	
	m_model->m_vertexBuffer->apply(0);
	m_model->m_indexBuffer->apply(0);
	
	m_shader->setMatrix("g_mWorldViewProj", worldViewProj);	

	D3D10_TECHNIQUE_DESC techDesc;
	m_shader->getTechnique()->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		m_shader->apply(p);
		m_device->DrawIndexed( m_model->m_size,0, 0);
	}
}