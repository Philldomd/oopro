#ifndef TERRAIN_H
#define TERRAIN_H
#include "d3dApp.h"
#include "Buffer.h"
#include "Shader.h"

using namespace std;

class Terrain
{
public:
	Terrain(ID3D10Device* p_device, UINT p_width, UINT p_height, int p_Y );
	virtual ~Terrain();

	void initialize(float p_width, float p_height, D3DXVECTOR2 p_terrainScale, Shader* p_shader);
	void render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection);

	UINT getWidth();
	UINT getHeight();
	float getGroundHeight();
	float getGroundWidth();
	int getY();
private:
	struct terrainVertex
	{
		D3DXVECTOR3 m_pos;
		D3DXVECTOR2 m_texCoord;
		D3DXVECTOR3 m_normal;
		
		terrainVertex(){ }
		terrainVertex(D3DXVECTOR3 p_pos, D3DXVECTOR3 p_norm, D3DXVECTOR2 p_texC)
		{
			m_pos = p_pos;
			m_normal = p_norm;
			m_texCoord = p_texC;
		}
	};

	UINT			m_height, m_width;
	float			m_groundHeight, m_groundWidth;
	int				m_Y;
	Buffer*			m_buffer;
	ID3D10Device*	m_device;

	Shader*			m_shader;
};

#endif