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
	float3 pos2 : mPOSITION;
    float3 norm2 : mNORMAL;
	float2 tex2 : mTEXTURE;
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
	float	 g_mLerpTime;
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
    CullMode = NONE;
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

	//Calc angle between the points

	float angle = acos(dot(normalize(input.pos),normalize(input.pos2)));
    //
    // Transform by our Sceneance matrix
    //
    //float4 ViewPos = mul(InstancePosition, g_mWorldView );
    angle *= 2;
    //
    // Transform the vert to view-space
    //
	float4 tempPos1, tempPos2;
	tempPos1 = mul(float4(input.pos, 1), g_mWorldViewProj);
	tempPos2 = mul(float4(input.pos2, 1), g_mWorldViewProj);

	float4 v4lerpPosition =	lerp(tempPos1, tempPos2, g_mLerpTime);
	//float3 lerpPosition = lerp(input.pos, input.pos2, g_mLerpTime);
	float3 slerpPosition;

	slerpPosition = ((sin((1-g_mLerpTime)*angle)/sin(angle))* input.pos) + (sin(g_mLerpTime*angle)/sin(angle))*input.pos2;

	float4 v4SlerpPosition;
	v4SlerpPosition = ((sin((1-g_mLerpTime)*angle)/sin(angle))* tempPos1) + (sin(g_mLerpTime*angle)/sin(angle))*tempPos2;

    float4 v4Position = mul(float4(slerpPosition, 1), g_mWorldViewProj);
    output.pos = v4SlerpPosition;
    
    //  
    // Transfer the rest
    //
    output.tex = lerp(input.tex, input.tex2, g_mLerpTime);
    
    //
    // dot the norm with the light dir
    //
    output.color = float4(1,1,1,1);//CalcLighting( norm, ViewPos.z );
    
    //
    // Dim the color by how far up the tree we are.  
    // This is a nice way to fake occlusion of the branches by the leaves.
    //
	float3 lerpNormal = lerp(input.norm, input.norm2, g_mLerpTime);

    output.color  = float4(1,1,1,1) * saturate(max(dot(float4(lerpNormal, 0), normalize(float4(0.0f, 1.0f, 0.0f, 0.0f))), 0.0f) + 0.05f);
    
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