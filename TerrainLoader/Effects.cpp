#include "Effects.h"
#include "Shader.h"

Shader* fx::TerrainFX		  = 0;

void fx::InitAll(ID3D10Device* device)
{
	const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(D3DXVECTOR3), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR2), D3D10_INPUT_PER_VERTEX_DATA, 0} 
	};

	TerrainFX = new Shader();
	TerrainFX->Init(device, "Terrain.fx", lineVertexLayout, sizeof(lineVertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "Color",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY);
}

void fx::DestroyAll()
{
	TerrainFX->~Shader();
	TerrainFX = NULL;
}