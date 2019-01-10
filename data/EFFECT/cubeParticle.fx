texture tex;
float4x4 mtxView;
float4x4 mtxProj;
float3 light1Dir;
float4 light1Diffuse;
float4 light1Ambient;
float3 light2Dir;
float4 light2Diffuse;
float4 light2Ambient;
float3 light3Dir;
float4 light3Diffuse;
float4 light3Ambient;

sampler tipSampler = sampler_state {
	texture = <tex>;
	MipFilter = LINEAR;
	MinFilter = POINT;
	MagFilter = POINT;
};

struct VS_OUT {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : COLOR0;
};

struct PS_OUTPUT {
	float4 RGB : COLOR0;
};

VS_OUT vsMain(
	float3 pos : POSITION,
	float2 localUV : TEXCOORD0,
	float3 normal : NORMAL,
	float4 mtxWorld1 : TEXCOORD1,
	float4 mtxWorld2 : TEXCOORD2,
	float4 mtxWorld3 : TEXCOORD3,
	float4 mtxWorld4 : TEXCOORD4,
	float4 col : COLOR0
) {
	VS_OUT Out;

	float4x4 mtxWorld = {
		mtxWorld1,
		mtxWorld2,
		mtxWorld3,
		mtxWorld4,
	};

	Out.pos = float4(pos, 1.0f);

	Out.pos = mul(Out.pos, mtxWorld);
	Out.pos = mul(Out.pos, mtxView);
	Out.pos = mul(Out.pos, mtxProj);

	Out.uv = localUV;

	float4x4 mtxWorldView = mul(mtxWorld, mtxView);
	Out.normal = mul(normal, mtxWorldView);

	Out.color = col;

	return Out;
}

float4 psMain( VS_OUT In ) : COLOR0 {

	float power1 = dot(normalize(In.normal), -normalize(light1Dir));
	power1 = clamp(power1, 0.0f, 1.0f);

	float power2 = dot(normalize(In.normal), -normalize(light2Dir));
	power2 = clamp(power2, 0.0f, 1.0f);

	float power3 = dot(normalize(In.normal), -normalize(light3Dir));
	power3 = clamp(power3, 0.0f, 1.0f);

	float4 RC = tex2D(tipSampler, In.uv) * In.color;
	float4 color = RC * (light1Diffuse * power1 + light1Ambient);
	color *= light2Diffuse * power2 + light2Ambient;
	color *= light3Diffuse * power3 + light3Ambient;
	color.a = 1.0f;
	return color;
}


technique tech {
	pass p0 {
		VertexShader = compile vs_2_0 vsMain();
		PixelShader  = compile ps_2_0 psMain();
	}
}
