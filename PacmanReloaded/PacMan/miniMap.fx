
Texture2D miniMap;

struct VS_INPUT
{
	float3 pos : POSITION;	
};

struct PS_INPUT2
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer cbEveryFrame
{
	matrix	g_mLightWVP;
};

RasterizerState backFaceCulling
{
	cullmode = back;
};
BlendState NoBlending
{
 AlphaToCoverageEnable = FALSE;
 BlendEnable[0] = FALSE;
};
SamplerState bilinearSampler
{
    Filter = min_mag_mip_linear;
    AddressU = MIRROR;
    AddressV = MIRROR;	
};
float4 VSInstmain( VS_INPUT input ) : SV_POSITION
{	
	return  float4(0,0,0,0);
}
float4 PS2( PS_INPUT2 input ) : SV_TARGET
{		
	return miniMap.Sample(bilinearSampler, input.uv); 	
}
[maxvertexcount(6)]
void GS( point float4 s[1] : SV_POSITION, inout TriangleStream<PS_INPUT2> triStream )
{
	PS_INPUT2 p;
	
	p.pos = float4(0.125f,-1,0,1);
	p.uv = float2(0,1);
	triStream.Append(p);
	
	p.pos = float4(0.125f,-0.125f,0,1);
	p.uv = float2(0,0);
	triStream.Append(p);

	p.pos = float4(1,-1,0,1);
	p.uv = float2(1,1);
	triStream.Append(p);

	p.pos = float4(1,-0.125f,0,1);
	p.uv = float2(1,0);
	triStream.Append(p);
}

technique10 RB
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VSInstmain() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
		SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetPixelShader( CompileShader( ps_4_0, PS2() ) );	
		SetRasterizerState( backFaceCulling );		
    }
}