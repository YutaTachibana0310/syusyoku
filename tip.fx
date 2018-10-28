float screenW;
float screenH;
texture tex;

sampler tipSampler = sampler_state {
	texture = <tex>;
	MipFilter = LINEAR;
	MinFilter = POINT;
	MagFilter = POINT;
};

struct VS_OUT {
	float4 pos : POSITION;
	float4 diffuse : DIFFUSE;
};

VS_OUT vsMain(
	float2 pos : POSITION,
	float2 localUV  : TEXCOORD0,
	float2 worldPos : TEXCOORD1,
	float2 tipUV    : TEXCOORD2,
) {
	VS_OUT Out; 
	Out.pos = float4(
				 ( pos.x + worldPos.x - screenW ) / screenW,
				-( pos.y + worldPos.y - screenH ) / screenH,
				0.0f,
				1.0f
			);
	Out.uv = tipUV + localUV;
	
	return Out;
}

float4 psMain( VS_OUT In ) : COLOR0 {
	return tex2D( tipSampler, In.uv );
}


technique tech {
	pass p0 {
		VertexShader = compile vs_2_0 vsMain();
		PixelShader  = compile ps_2_0 psMain();
	}
}
