#include "Effects.h"
#include "Shader.h"

//Shader* fx::TerrainFX		  = 0;
Shader* fx::InstanceFX		= 0;

void fx::InitAll(ID3D10Device* device)
{
	/*const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(D3DXVECTOR3), D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR2), D3D10_INPUT_PER_VERTEX_DATA, 0} 
	};

	TerrainFX = new Shader();
	TerrainFX->init(device, "Terrain.fx", lineVertexLayout, sizeof(lineVertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "Color",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY);*/


	// Define our instanced vertex data layout
    const D3D10_INPUT_ELEMENT_DESC instlayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "mTransform", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
        { "mTransform", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
        { "mTransform", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
        { "mTransform", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
    };

    /*int iNumElements = sizeof( instlayout ) / sizeof( D3D10_INPUT_ELEMENT_DESC );
    D3D10_PASS_DESC PassDesc;
    ID3D10EffectPass* pPass = g_pRenderInstancedVertLighting->GetPassByIndex( 0 );
    pPass->GetDesc( &PassDesc );*/

	InstanceFX = new Shader();
	InstanceFX->init(device, "Instancing.fx", instlayout, sizeof(instlayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "RenderInstancedVertLighting",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY);
}

void fx::DestroyAll()
{
	/*TerrainFX->~Shader();
	TerrainFX = NULL;*/
}