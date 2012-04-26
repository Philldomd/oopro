struct VS_INPUT
{
	float4 pos : POSITION;	
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct PS_INPUT2
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

RasterizerState backFaceCulling
{
	cullmode = back;
};
Texture2D miniMap;
SamplerState bilinearSampler
{
    Filter = min_mag_mip_linear;
    AddressU = MIRROR;
    AddressV = MIRROR;	
};
float4 VS2( VS_INPUT input ) : SV_POSITION
{
	return float4(0,0,1,1);
}
float4 PS2( PS_INPUT2 input ) : SV_TARGET
{		
	float r = miniMap.Sample(bilinearSampler, input.uv).r;
	return float4(r,r,r,1);	
}
[maxvertexcount(6)]
void GS( point float4 s[1] : SV_POSITION, inout TriangleStream<PS_INPUT2> triStream )
{
	PS_INPUT2 p;
	
	p.pos = float4(-1,0.50,0,1);
	p.uv = float2(0,1);
	triStream.Append(p);
	
	p.pos = float4(-1,1,0,1);
	p.uv = float2(0,0);
	triStream.Append(p);

	p.pos = float4(-0.50,0.50,0,1);
	p.uv = float2(1,1);
	triStream.Append(p);

	p.pos = float4(-0.50,1,0,1);
	p.uv = float2(1,0);
	triStream.Append(p);
}

technique10 RenderBillboard
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS2() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );
        SetPixelShader( CompileShader( ps_4_0, PS2() ) );	
		SetRasterizerState( backFaceCulling );		
    }
}