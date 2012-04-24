cbuffer cbPerObject
{
	float4x4 gWVP; 
	float4x4 gWorld;
	float3 gEyePosW;	
};

cbuffer cbFixed
{
	float gTexScale;
};

SamplerState gTriLinearSam
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};

BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};


struct VS_IN
{
	float3 posL    : POSITION;
	float2 texC    : TEXCOORD;
	float3 normalL : NORMAL;
};

struct VS_OUT
{
	float4 posH         : SV_POSITION;
	float4 normalW		: NORMAL;
    float2 stretchedUV  : TEXCOORD1;	
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);

	vOut.normalW = mul(float4(vIn.normalL, 0.0f), gWorld);

	
	vOut.stretchedUV = vIn.texC;

	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	return float4(1,1,1,1) * saturate(max(dot(pIn.normalW, normalize(float4(0.5,0.5,0.5,0))), 0.0f) + 0.25f);
	//return float4(pIn.stretchedUV.x,pIn.stretchedUV.y,0,1); 
}

RasterizerState Wireframe
{
        FillMode = Solid;
        CullMode = NONE;
        FrontCounterClockwise = false;
};

technique10 Color
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
        SetBlendState(NoBlend, float4(0.0f,0.0f,0.0f,0.0f), 0xFFFFFFFF);
        SetRasterizerState(Wireframe);
    }
}