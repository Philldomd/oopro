//--------------------------------------------------------------------------------------
// Box.fx
// Direct3D 10 Shader Model 4.0 Debug Drawing for Bounding Boxes
// Copyright (c) Mattias Fridh, 2012
//--------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 Pos	: POS;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
};

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
	float4	gColor;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepthTestOnly
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};

RasterizerState Wireframe
{
	FillMode = WireFrame;
	CullMode = NONE;
	//FrontCounterClockwise = true;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.Pos = mul(float4(input.Pos,1.0), g_mWorldViewProjection );

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	return gColor;
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique10 DrawLine
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );

		//SetBlendState(NoBlend, float4(0.0f,0.0f,0.0f,0.0f), 0xFFFFFFFF);
	    SetRasterizerState( Wireframe );

	    SetDepthStencilState( EnableDepth, 0 );
	    //SetDepthStencilState( DisableDepth, 0 );
	    //SetDepthStencilState( EnableDepthTestOnly, 0 );
    }  
}