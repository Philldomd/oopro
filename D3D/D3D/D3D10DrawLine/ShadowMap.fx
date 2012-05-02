matrix gLightWVP;

float4 VS(float3 posL : POSITION):SV_POSITION
{
	//Render from light's perspective
	return mul(float4(posL, 1.0f), gLightWVP);
}



BlendState NoBlend
{
	BlendEnable[0] = false;
};

RasterizerState rs
{
	FillMode = Solid;
	CullMode = Front;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

technique10 RenderShadowMap
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
		SetDepthStencilState( EnableDepth, 0);
		SetBlendState( NoBlend, float4( 0.0f,0.0f,0.0f,0.0f), 0xFFFFFFFF);
		SetRasterizerState(rs);
	}
}