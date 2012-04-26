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
    row_major float4x4 mTransform : mTransform;
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
cbuffer crarely
{
    float4x4 g_mTreeMatrices[50];
    uint g_iNumTrees;
};

cbuffer ceveryframe
{
    float4x4 g_mWorldViewProj;
    float4x4 g_mWorldView;
	float4x4 g_mRotation;
};

//cbuffer cmultipleperframe
//{
    //float g_GrassWidth;
    //float g_GrassHeight;
    //uint g_iGrassCoverage;
//};
//
//cbuffer cusercontrolled
//{
    //float g_GrassMessiness;
//};

struct light_struct
{
    float4 direction;
    float4 color;
};

cbuffer cimmutable
{
    light_struct g_lights[4] = { 
                    { float4(0.620275,  0.683659, 0.384537, 1),  float4(0.75, 0.599, 0.405, 1) },		//sun
                    { float4(0.063288, -0.987444, 0.144735, 1),  float4(0.192, 0.273, 0.275, 1) },		//bottom
                    { float4(0.23007,   0.785579, -0.574422, 1),  float4(0.300, 0.292, 0.223, 1) },		//highlight
                    { float4(-0.620275,  -0.683659, -0.384537, 1),  float4(0.0, 0.0, 0.1, 1) }			//blue rim-light
                    };
    
    float4 g_ambient = float4(0.4945,0.465,0.5,1);
    
    float g_occDimHeight = 2400.0;	//scalar that tells us how much to darken the tree near the top
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D g_txDiffuse;
Texture2DArray g_tx2dArray;
SamplerState g_samLinear
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};

Texture1D g_txRandom;
SamplerState g_samPoint
{
    Filter = MIN_MAG_MIP_POINT;
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
// CalcLighting helper function.  Calculates lighting from 4 light sources, adds ambient
// and attenuates for depth.  Used by all techniques for lighting.
//--------------------------------------------------------------------------------------
float4 CalcLighting( float3 norm, float depth )
{
    float4 color = float4(0,0,0,0);
    
    // add the contributions of 4 directional lights
    [unroll] for( int i=0; i<4; i++ )
    {
        color += saturate( dot(g_lights[i].direction,float4(norm,0)) )*g_lights[i].color;
    }
    
    // give some attenuation due to depth
    float attenuate = depth / 10000.0;
    float4 attenColor = float4(0.15, 0.2, 0.3, 0);
    
    // add it all up plus ambient
    return (1-attenuate*0.23)*(color + g_ambient) + attenColor*attenuate;
}

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
    float4 InstancePosition = mul(float4(input.pos, 1), mul(g_mRotation,input.mTransform));
    //float4 ViewPos = mul(InstancePosition, g_mWorldView );
    
    //
    // Transform the vert to view-space
    //
    float4 v4Position = mul(InstancePosition, g_mWorldViewProj);
    output.pos = v4Position;
    
    //  
    // Transfer the rest
    //
    output.tex = input.tex;
    
    //
    // dot the norm with the light dir
    //
    float3 norm = mul(input.norm,(float3x3)input.mTransform);
    output.color = float4(1,1,1,1);//CalcLighting( norm, ViewPos.z );
    
    //
    // Dim the color by how far up the tree we are.  
    // This is a nice way to fake occlusion of the branches by the leaves.
    //
    output.color  = float4(1,1,1,1) * saturate(max(dot(float4(norm, 0), normalize(float4(0.0f, 1.0f, 0.0f, 0.0f))), 0.0f) + 0.05f);
    
    return output;
}

//--------------------------------------------------------------------------------------
// PS for non-leaf or grass items.
//--------------------------------------------------------------------------------------
float4 PSScenemain(PSSceneIn input) : SV_Target
{
    float4 color = g_txDiffuse.Sample( g_samLinear, input.tex ) * input.color;
    return input.color;
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