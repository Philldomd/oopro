//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
Texture2D myTexture;
Texture2D myShaderTex;
static const float SHADOW_EPSILON = 0.001f;
float SMAP_SIZE_X;
float SMAP_SIZE_Y;

struct VSSceneIn
{
	float3 Pos	: POS;
	float2 TexC : TEXCOORD;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float2 TexC : TEXCOORD;
	float4 lpos : POS0;
	float4 wpos : POS1;
};

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
	matrix	g_mLightWVP;
};
//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	FillMode = SOLID;
	CullMode = FRONT;
};
RasterizerState WireFrame
{
	FillMode				= WIREFRAME; //WIREFRAME,SOLID
	CullMode				= NONE;
};
SamplerState linearSampler 
{
	Filter = ANISOTROPIC; //MIN_MAG_MIP_LINEAR
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState shadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;

};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.wpos = float4(input.Pos, 1.0);
	output.Pos = mul( float4(input.Pos,1.0), g_mWorldViewProjection );
	output.lpos = mul(float4(input.Pos, 1.0f), g_mLightWVP);
	//Pass through
	output.TexC = input.TexC;
	
	return output;
}
//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	float4 texColor = myTexture.Sample(linearSampler, input.TexC);

	//Project the texture coords and scale/offset to [0,1]
	input.lpos.xy /= input.lpos.w;

	//Compute shadow map tex coord
	float2 smTex = float2( 0.5f*input.lpos.x, -0.5f*input.lpos.y) + 0.5f;

	//Compute pixel depth for shadowing
	float depth = input.lpos.z / input.lpos.w;

	//2x2 percentage closest filter.
	float dx = 1.0f / SMAP_SIZE_X;
	float dy = 1.0f / SMAP_SIZE_Y;
	float s0 = (myShaderTex.Sample(shadowMapSampler, smTex).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s1 = (myShaderTex.Sample(shadowMapSampler, smTex + float2(dx, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s2 = (myShaderTex.Sample(shadowMapSampler, smTex + float2(0.0f, dy)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s3 = (myShaderTex.Sample(shadowMapSampler, smTex + float2(dx, dy)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;

	//Transform to texel space
	float2 texelPos = float2(smTex.x * SMAP_SIZE_X, smTex.y * SMAP_SIZE_Y);

	//Determine the lert amounts
	float2 lerps = frac(texelPos);

	float shadowCoeff = lerp( lerp( s0, s1, lerps.x), lerp( s2, s3, lerps.x), lerps.y);

	float3 litColor = texColor.rgb * shadowCoeff;

	return float4(litColor, texColor.a);
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
    }  
}

