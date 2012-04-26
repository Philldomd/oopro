//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct Light
{
	float3 pos;
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 att; // attenuation parameters (a0, a1, a2)
	float range;
};


Texture2D myTexture;
struct VSSceneIn
{
	float3 Pos	: POS;
	float2 TexC : TEXCOORD;
	float3 Normal : NORMAL;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float3 PosW	: POS;
	float2 TexC : TEXCOORD;
	float3 Normal : NORMAL;
};

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
	matrix	g_mWorldIT;
};

cbuffer cbFixed
{
	Light L = 
	{
		float3(0.0f,100.0f,0.0f), //Pos
		float4(0.8f,0.8f,0.8f,1.0f), // Ambient
		float4(1.0f,1.0f,1.0f,1.0f), // Diffuse
		float4(1.0f,1.0f,1.0f,1.0f), // Spec
		float3(0.0f,0.02f,0.0f), //Att

		1000.0f //range
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

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
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

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	// The vector from the surface to the light.
	float3 lightVec = L.pos - v.pos;
	// The distance from surface to light.
	float d = length(lightVec);
	if( d > L.range )
		return float3(0.0f, 0.0f, 0.0f);
	// Normalize the light vector.
	lightVec /= d;
	// Add the ambient light term.
	litColor += v.diffuse * L.ambient;
	// Add diffuse and specular term, provided the surface is in
	// the line of sight of the light.
	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower = max(v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos - v.pos);
		float3 R = reflect(-lightVec, v.normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse * L.diffuse;
		litColor += specFactor * v.spec * L.spec;
	}
	// attenuate
	return litColor / dot(L.att, float3(1.0f, d, d*d));
}
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.PosW = input.Pos;
	output.Pos = mul( float4(input.Pos,1.0), g_mWorldViewProjection );
	//Pass through
	output.TexC = input.TexC;
	output.Normal = mul(float4(input.Normal, 0.0f), g_mWorldIT);
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float4 Tex = myTexture.Sample(linearSampler, input.TexC);
	float3 eyePos = float3(0.0f,0.0f,-200.0f);

	SurfaceInfo v = {input.PosW, input.Normal, Tex, Tex};

	return float4(PointLight(v,L ,eyePos), Tex.a);
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
		//SetRasterizerState( WireFrame );

	    SetDepthStencilState( EnableDepth, 0 );
    }  
}

