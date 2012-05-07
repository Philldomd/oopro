//--------------------------------------------------------------------------------------
// File: Instancing.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Input and output structures 
//--------------------------------------------------------------------------------------
struct VSInstIn
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
	float3 norm : NORMAL;
};

struct PSSceneIn
{
	float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
    float3 norm : NORMAL;
};

//--------------------------------------------------------------------------------------
// Constant buffers 
//--------------------------------------------------------------------------------------
cbuffer ceveryframe
{
    float4x4 g_mWorldViewProj;
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse;

SamplerState g_samLinear
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};
//--------------------------------------------------------------------------------------
// State structures
//--------------------------------------------------------------------------------------
RasterizerState EnableMSAA
{
	FillMode = SOLID;
    CullMode = FRONT;
};

DepthStencilState EnableDepthTestWrite
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};
//--------------------------------------------------------------------------------------
// Instancing vertex shader.  Positions the vertices based upon the matrix stored
// in the second vertex stream.
//--------------------------------------------------------------------------------------
PSSceneIn VSInstmain(VSInstIn input)
{
    PSSceneIn output;
    //float4 ViewPos = mul(InstancePosition, g_mWorldView );
    
    //
    // Transform the vert to view-space
    //
    output.pos = mul(float4(input.pos, 1.0f), g_mWorldViewProj);
    
    //  
    // Transfer the rest
    //
    output.tex = input.tex;
	output.norm = input.norm;

    return output;
}

//--------------------------------------------------------------------------------------
// PS for non-leaf or grass items.
//--------------------------------------------------------------------------------------
float4 PSScenemain(PSSceneIn input) : SV_Target
{
	return float4(1,0,1,1);
	//return g_txDiffuse.Sample( g_samLinear, input.tex );
}

//--------------------------------------------------------------------------------------
// Render instanced meshes with vertex lighting
//--------------------------------------------------------------------------------------
technique10 RenderTerrain
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSInstmain() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScenemain() ) );
        
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( EnableDepthTestWrite, 0 );
        SetRasterizerState( EnableMSAA );
    }  
}