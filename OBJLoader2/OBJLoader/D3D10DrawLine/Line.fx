//--------------------------------------------------------------------------------------
// Particle.fx
// Direct3D 10 Shader Model 4.0 Line Drawing Demo
// Copyright (c) Stefan Petersson, 2008
//--------------------------------------------------------------------------------------
struct Light
{
	float3 pos;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att;//a0,a1,a2
	float range;
};
//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 Pos			: POS;
	float3 Norm			: NORMAL;
	float2 texCoord		: TEXCOORD;
};
Texture2D	DDSPIC;
SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
struct PSSceneIn
{
	float4 Pos		: SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float3 PosW		: POS;
	float3 Norm		: NORMAL;
	float2 texCoord	: TEXCOORD;
};

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
	matrix	mNormalMatrix;
};

cbuffer cbLight
{
	Light L = {
	float3(0,200,0),
	float4(0.2f,0.2f,0.2f,1),
	float4(1.0f,1.0f,1.0f,1),
	float4(5.0f,5.0f,5.0f,1),
	float3(0,0.02f,0),//a0,a1,a2
	10000
	};
};
//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
struct SurfaceInfo
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
};
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
};

RasterizerState NoCulling
{
	FillMode = SOLID;//WIREFRAME; //SOLID
	CullMode = NONE;
};

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f,0.0f,0.0f);

	float3 lightVec = L.pos - v.pos;

	float d = length(lightVec);

	if( d > L.range)
		return float3(0.0f,0.0f,0.0f);

	lightVec /= d;

	litColor += v.diffuse * L.ambient;

	float diffuseFactor = dot(lightVec, v.normal);

	if( diffuseFactor > 0.0f)
	{
		float specPower		= max(v.spec.a, 1.0f);
		float3 toEye		= normalize(eyePos - v.pos);
		float3 R			= reflect(-lightVec, v.normal);
		float specFactor	= pow(max(dot(R, toEye), 0.0f), specPower);

		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}

	return litColor / dot(L.att, float3(1.0f,d,d*d));
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.PosW = mul (float4(input.Pos, 1.0f), mNormalMatrix);
	output.Pos = mul( float4(input.Pos,1.0), g_mWorldViewProjection );
	output.texCoord = input.texCoord;
	output.Norm = mul(float4(input.Norm, 1.0f), mNormalMatrix);
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float3 eyePos = float3(0,100,-200);
	float4 tex = DDSPIC.Sample(linearSampler, input.texCoord);
	SurfaceInfo v = {input.PosW, input.Norm,tex,tex};
	
	return float4(PointLight(v,L,eyePos), tex.a);
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
    }  
}

