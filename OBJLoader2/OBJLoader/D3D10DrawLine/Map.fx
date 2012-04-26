//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 Pos			: POS;
	float3 Norm			: NORMAL;
	float2 texCoord		: TEXCOORD;
};
Texture2D	GRASS;
Texture2D	SHADOW;

SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};
struct PSSceneIn
{
	float4 Pos		: SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float4 PosW		: POS;
	float4 lpos		: LPOS;
	float3 Norm		: NORMAL;
	float2 texCoord	: TEXCOORD;
};
matrix	gLightWVP;
//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
	matrix	mNormalMatrix;
	float SHADOW_EPSILON = 0.001f;
	float SMAP_SIZE_X = 1024;
	float SMAP_SIZE_Y = 1024;
	float4	ambient = float4(0.8f,0.8f,0.8f,1.0f);
	float4	diffuse = float4(1.0f,1.0f,1.0f,1);
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
};

RasterizerState NoCulling
{
	FillMode = SOLID;//WIREFRAME; //SOLID
	CullMode = NONE;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.PosW = float4(input.Pos, 1.0);
	output.Pos = mul( float4(input.Pos,1.0), g_mWorldViewProjection );
	output.Norm = input.Norm;
	output.texCoord = input.texCoord;
	output.lpos = mul( float4(input.Pos, 1.0), gLightWVP); 
	
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float4 texColor = GRASS.Sample(linearSampler, input.texCoord);

	//Project the texture coords and scale/offset to [0,1]
	input.lpos.xy /= input.lpos.w;

	//Compute shadow map tex coord
	float2 smTex = float2( 0.5f*input.lpos.x, -0.5f*input.lpos.y) + 0.5f;

	//Compute pixel depth for shadowing
	float depth = input.lpos.z / input.lpos.w;

	//2x2 percentage closest filter.
	float dx = 1.0f / SMAP_SIZE_X;
	float dy = 1.0f / SMAP_SIZE_Y;
	float s0 = (SHADOW.Sample(linearSampler, smTex).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s1 = (SHADOW.Sample(linearSampler, smTex + float2(dx, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s2 = (SHADOW.Sample(linearSampler, smTex + float2(0.0f, dy)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s3 = (SHADOW.Sample(linearSampler, smTex + float2(dx, dy)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;

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

	    SetDepthStencilState( EnableDepth, 0 );
    }  
}

