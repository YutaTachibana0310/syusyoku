sampler tex0 : register(s0);

struct VS_OUTPUT {
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(
	float4 Pos : POSITION,
	float4 Col : COLOR0,
	float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.Pos = Pos;
	Out.Tex = Tex;

	return Out;
}

float4 PS(VS_OUTPUT In) : COLOR0
{
	float4 Col = tex2D(tex0, In.Tex);

	Col.rgb = (Col.r + Col.g + Col.b) * 0.333f;

	return Col;
}

technique tech {
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS();
	}
}