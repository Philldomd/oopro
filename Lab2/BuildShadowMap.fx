//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
Texture2D shadowMap;
struct VSSceneIn
{
	float3 Pos	: POS;
};
struct PSSceneIn
{
	float4 Pos : SV_POSITION;	
};
struct PS_INPUT2
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};
//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mLightWVP;
};
//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

RasterizerState rs
{
	FillMode = SOLID;
	CullMode = FRONT;
};
RasterizerState rs2
{
	//FillMode = SOLID;
	CullMode = BACK;
};
BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};
SamplerState bilinearSampler
{
    Filter = min_mag_mip_linear;
    AddressU = MIRROR;
    AddressV = MIRROR;	
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
float4 VSScene(VSSceneIn input) : SV_POSITION
{
	//Render from light's perspective
	return  mul(float4(input.Pos, 1.0f), g_mLightWVP);
}
float4 VS2( VSSceneIn input ) : SV_POSITION
{
	return float4(0,0,0,0);
}
//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PS2( PS_INPUT2 input ) : SV_TARGET
{		
	float r = shadowMap.Sample(bilinearSampler, input.uv).r;
	return float4(r, r, r, 1);	
}
//--------------------------------------------------------------------------------------
// Geometry Shader - Billboard
//--------------------------------------------------------------------------------------

[maxvertexcount(6)]
void GS( point float4 s[1] : SV_POSITION, inout TriangleStream<PS_INPUT2> triStream )
{
	PS_INPUT2 p;
	
	p.pos = float4(-1, 0.50, 0, 1);
	p.uv = float2(0,1);
	triStream.Append(p);
	
	p.pos = float4(-1, 1, 0, 1);
	p.uv = float2(0,0);
	triStream.Append(p);

	p.pos = float4(-0.50, 0.50, 0, 1);
	p.uv = float2(1,1);
	triStream.Append(p);

	p.pos = float4(-0.50, 1, 0, 1);
	p.uv = float2(1,0);
	triStream.Append(p);
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
        SetPixelShader( NULL );
	    
	    SetRasterizerState( rs );
		SetBlendState(NoBlend, float4(0.0f,0.0f,0.0f,0.0f), 0xFFFFFFFF);
	    SetDepthStencilState( EnableDepth, 0 );
    }  
}

technique10 RenderBillboard
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS2() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS2() ) );	
		SetRasterizerState( rs2 );		
    }
}