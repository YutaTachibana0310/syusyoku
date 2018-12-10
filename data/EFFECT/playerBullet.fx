texture tex;			//テクスチャ
float4x4 mtxView;		//ビュー行列
float4x4 mtxProj;		//プロジェクション行列

//テクスチャサンプラー
sampler texSampler = sampler_state {
	texture = <tex>;
	MipFilter = LINEAR;
	MinFilter = POINT;
	MagFilter = POINT;
};

//頂点シェーダ出力構造体
struct VS_OUT {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
	float4 color : COLOR0;
};

//ピクセルシェーダ出力構造体
struct PS_OUTPUT {
	float4 RGB : COLOR0;
};

//頂点シェーダ
VS_OUT vsMain(
	float3 pos : POSITION,
	float2 localUV : TEXCOORD0,
	float4 mtxWorld1 : TEXCOORD1,
	float4 mtxWorld2 : TEXCOORD2,
	float4 mtxWorld3 : TEXCOORD3,
	float4 mtxWorld4 : TEXCOORD4,
	float col : TEXCOORD5
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
	
	Out.color = col;

	return Out;
}

//ピクセルシェーダ
float4 psMain( VS_OUT In ) : COLOR0 {

	return tex2D(texSampler, In.uv) * In.color;
}

technique tech {
	pass p0 {
		VertexShader = compile vs_2_0 vsMain();
		PixelShader  = compile ps_2_0 psMain();
	}
}
