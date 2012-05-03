
cbuffer cbPerObject
{
	matrix gWVP; 
	float4 gDirToSunW = normalize(float4( 0.0f, 1.0f, 1.0f, 0.0f ));
	matrix gWorld;
	float4 gColor = float4(1,1,1,1);
};

Texture2D gTexture;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
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
	float3 normalL : NORMAL;
	float2 texC    : TEXCOORD;
};

struct VS_OUT
{
	float4 posH         : SV_POSITION;
	float shade         : SHADE;
    float2 stretchedUV  : TEXCOORD1; 
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);

	float4 normalW = mul(float4(vIn.normalL, 0.0f), gWorld);
	vOut.shade = saturate(max(dot(normalW, gDirToSunW), 0.0f) + 0.25f);

	//vOut.tiledUV = gTexScale*vIn.texC;
	vOut.stretchedUV = vIn.texC;

	return vOut;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	float4 C = gTexture.Sample( gTriLinearSam, pIn.stretchedUV );
	C *= gColor;
	C *= pIn.shade;
	
    return C;
}

RasterizerState Wireframe
{
        FillMode = Solid;
        CullMode = Back;
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