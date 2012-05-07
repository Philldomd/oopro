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
    float3 norm : NORMAL;
    float2 tex : TEXTURE0;
};

struct PSSceneIn
{
    float4 pos : SV_Position;
    float2 tex : TEXTURE0;
    float4 color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Constant buffers 
//--------------------------------------------------------------------------------------

cbuffer ceveryframe
{
    float4x4 g_mWorldViewProj;
    float4x4 g_mWorldView;
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
    CullMode = BACK;
    MultisampleEnable = TRUE;
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
    
    //
    // Transform by our Sceneance matrix
    //
    //float4 ViewPos = mul(InstancePosition, g_mWorldView );
    
    //
    // Transform the vert to view-space
    //
    float4 v4Position = mul(float4(input.pos, 1), g_mWorldViewProj);
    output.pos = v4Position;
    
    //  
    // Transfer the rest
    //
    output.tex = input.tex;
    
    //
    // dot the norm with the light dir
    //
    output.color = float4(1,1,1,1);//CalcLighting( norm, ViewPos.z );
    
    //
    // Dim the color by how far up the tree we are.  
    // This is a nice way to fake occlusion of the branches by the leaves.
    //
    output.color  = float4(1,1,1,1) * saturate(max(dot(float4(input.norm, 0), normalize(float4(0.0f, 1.0f, 0.0f, 0.0f))), 0.0f) + 0.05f);
    
    return output;
}

//--------------------------------------------------------------------------------------
// PS for non-leaf or grass items.
//--------------------------------------------------------------------------------------
float4 PSScenemain(PSSceneIn input) : SV_Target
{
	float4 color;
	//color = g_txDiffuse.Sample( g_samLinear, input.tex ) * input.color;

	color = input.color;
    return color;
}

//--------------------------------------------------------------------------------------
// Render instanced meshes with vertex lighting
//--------------------------------------------------------------------------------------
technique10 RenderInstancedVertLighting
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