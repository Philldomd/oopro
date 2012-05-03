//--------------------------------------------------------------------------------------
// Particle.fx
// Direct3D 10 Shader Model 4.0 Line Drawing Demo
// Copyright (c) Stefan Petersson, 2008, Modified by Philip Andersson 2012
//--------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 Pos		: POS;
	float3 Norm		: NORMAL;
	float2 texCoord : TEXCOORD;
};

Texture2D	GRASS;
SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct PSSceneIn
{
	float4 Pos		: SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float3 Norm		: NORMAL;
	float2 texCoord	: TEXCOORD;
	float4 EyePos	: EYEPOS;
};

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
	matrix	mEyeWorldView;	

	float4	LPos;
	float3	LCol;
	float	Ld;
};
float4 AmbientLight()
{
	float intensity = 0.10f;
	float4 color = float4(1.0f,1.0f,1.0f,0.0f);
	return intensity * color;
}
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
BlendState NoBlending
{
 AlphaToCoverageEnable = FALSE;
 BlendEnable[0] = FALSE;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.Pos = mul( float4(input.Pos,1.0), g_mWorldViewProjection );
	output.texCoord = input.texCoord;
	output.Norm = input.Norm;
	output.EyePos = mul( float4(input.Pos,1.0), mEyeWorldView );
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	float4 S = normalize(float4(LPos - input.EyePos));
	float4 N = normalize(float4(input.Norm,0));
	float4 Kd = GRASS.Sample(linearSampler, input.texCoord);
	float4 L = Ld * Kd * max(dot(S,N),0);
	float4 La = AmbientLight() * Kd;	
	return L + La;
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
	    
	    SetRasterizerState( NoCulling );

	    SetDepthStencilState( EnableDepth, 0 );
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
	    //SetDepthStencilState( DisableDepth, 0 );
	    //SetDepthStencilState( EnableDepthTestOnly, 0 );
    }  
}

