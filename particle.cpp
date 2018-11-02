#include "particle.h"
#include "camera.h"
#include "debugproc.h"

/**********************************************
マクロ定義
**********************************************/
#define PARTICLE_MAX (1024*16)
#define PARTICLE_TEXNAME "data/TEXTURE/explosionSmog.png"
#define PARTICLE_LUMINETEX	"explosionLumine.png"
#define PARTICLE_SIZE (2)
#define USE_SHADER (1)
#define PARTICLE_RANGE	(2000)
#define USE_ALPHATEST (0)

/**********************************************
グローバル変数
**********************************************/
static D3DXMATRIX pos[PARTICLE_MAX];		//ワールド変換行列の配列
static COLOR vtxColor[PARTICLE_MAX];		//頂点ディフューズの配列
static UV vtxUV[PARTICLE_MAX];				//テクスチャ座標の配列

//単位パーティクルの各頂点設定
static PARTICLE_VTX vtx[4] = {
	{-PARTICLE_SIZE, PARTICLE_SIZE, 0.0f, 0.0f, 0.0f},
	{ PARTICLE_SIZE, PARTICLE_SIZE, 0.0f, 1.0f, 0.0f },
	{ -PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f,  0.0f, 1.0f },
	{ PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f, 1.0f, 1.0f }
};

//各種の頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 uvBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 posBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 colorBuff = NULL;

static LPDIRECT3DVERTEXDECLARATION9 declare;	//要素宣言（シェーダへの引数）
static LPDIRECT3DTEXTURE9 texture, lumine;		//テクスチャ
static LPD3DXEFFECT effect;						//シェーダ
static LPDIRECT3DINDEXBUFFER9 indexBuff;		//インデックスバッファ
static PARTICLE_HOGE particle[PARTICLE_MAX];			//パーティクル構造体
static int cntParticle = 0;						//パーティクルカウント
static int cntFrame = 0;						//フレームカウント

/**********************************************
プロトタイプ宣言
**********************************************/
void CopyVtxBuff(unsigned size, void *src, LPDIRECT3DVERTEXBUFFER9 buff);
float RandomRange(float min, float max);

/**********************************************
初期化処理
**********************************************/
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//配列初期化
	for (int i = 0; i < PARTICLE_MAX; i++)
	{
		particle[i].pos = D3DXVECTOR3(-99999, -99999, -99999);
		D3DXMatrixIdentity(&pos[i]);

		vtxUV[i].u = vtxUV[i].v = 0.0f;

		vtxColor[i].r = vtxColor[i].g = vtxColor[i].g = 1.0f;
		vtxColor[i].a = 0.0f;

		particle[i].active = false;
	}

	//頂点バッファ作成
	pDevice->CreateVertexBuffer(sizeof(vtx), 0, 0, D3DPOOL_MANAGED, &vtxBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(D3DXMATRIX) * PARTICLE_MAX, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &posBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(UV) * PARTICLE_MAX, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &uvBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(COLOR) * PARTICLE_MAX, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &colorBuff, 0);

	//頂点バッファにメモリコピー
	CopyVtxBuff(sizeof(vtx), vtx, vtxBuff);
	CopyVtxBuff(sizeof(D3DXMATRIX) * PARTICLE_MAX, pos, posBuff);
	CopyVtxBuff(sizeof(UV) * PARTICLE_MAX, vtxUV, uvBuff);
	CopyVtxBuff(sizeof(COLOR) * PARTICLE_MAX, vtxColor, colorBuff);

	//宣言作成（シェーダへの引数になる）
	D3DVERTEXELEMENT9 declareElems[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
		{ 3, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration(declareElems, &declare);

	//インデックスバッファ作成
	WORD index[6] = { 0, 1, 2, 2, 1, 3 };
	pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
	void *p = NULL;
	indexBuff->Lock(0, 0, &p, 0);
	memcpy(p, index, sizeof(index));
	indexBuff->Unlock();

	//シェーダー読み込み
	ID3DXBuffer *errBuf = 0;
	D3DXCreateEffectFromFile(pDevice, "data/EFFECT/particle.fx", 0, 0, 0, 0, &effect, &errBuf);
	if (errBuf)
	{
		//エラー処理
	}

	//テクスチャ読み込み
	texture = CreateTextureFromFile((LPSTR)PARTICLE_TEXNAME, pDevice);
	lumine = CreateTextureFromFile((LPSTR)PARTICLE_LUMINETEX, pDevice);
	
}

/**********************************************
終了処理
**********************************************/
void UninitParticle(void)
{
	SAFE_RELEASE(vtxBuff);
	SAFE_RELEASE(uvBuff);
	SAFE_RELEASE(posBuff);
	SAFE_RELEASE(colorBuff);
	SAFE_RELEASE(declare);
	SAFE_RELEASE(texture);
	SAFE_RELEASE(effect);
}

/**********************************************
更新処理
**********************************************/
void UpdateParticle(void)
{
	PARTICLE_HOGE *ptr = &particle[0];
	D3DXMATRIX mtxTranslate, mtxScale;

	cntFrame++;

#if 0
	//吸い込まれていくrパーティクルのセット
	if (cntFrame < 180)
	{
		for (int i = 0; i < 80; i++)
		{
			SetCircleParticle();
		}
	}
	//弾けるパーティクルのセット
	else if (cntFrame > 300 && cntFrame < 480)
	{
		for (int i = 0; i < 80; i++)
		{
			SetParticle();
		}
	}
	//ループ処理
	else if (cntFrame == 660)
	{
		cntFrame = 0;
	}
#endif
	//書くパーティクルの更新
	for (int i = 0; i < PARTICLE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//パーティクルの移動、透過
		ptr->pos += ptr->moveDir * ptr->speed;
		vtxColor[i].a = (1.0f - (float)ptr->cntFrame / ptr->lifeFrame) * 0.5f;
		ptr->cntFrame++;

		//寿命が来ていたら見えなくする
		if (ptr->cntFrame == ptr->lifeFrame)
		{
			ptr->pos.z = -10000.0f;
			cntParticle--;
			ptr->active = false;
		}

		//座標に応じたワールド変換行列にpos配列を更新
		D3DXMatrixIdentity(&pos[i]);
		GetInvCameraRotMtx(&pos[i]);
		D3DXMatrixScaling(&mtxScale, ptr->scale, ptr->scale, ptr->scale);
		D3DXMatrixMultiply(&pos[i], &pos[i], &mtxScale);
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&pos[i], &pos[i], &mtxTranslate);
	}

	//頂点バッファにメモリコピー
	CopyVtxBuff(sizeof(D3DXMATRIX) * PARTICLE_MAX, pos, posBuff);
	CopyVtxBuff(sizeof(COLOR) * PARTICLE_MAX, vtxColor, colorBuff);

	PrintDebugProc("Particle:%d\n", cntParticle);
}

/**********************************************
描画処理
**********************************************/
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX world, translate, mtxRot;

	//αテスト使用
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//ストリーム周波数を設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (PARTICLE_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点宣言設定
	pDevice->SetVertexDeclaration(declare);

	//ストリームソース設定
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(PARTICLE_VTX));
	pDevice->SetStreamSource(1, posBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetStreamSource(2, uvBuff, 0, sizeof(UV));
	pDevice->SetStreamSource(3, colorBuff, 0, sizeof(COLOR));
	pDevice->SetIndices(indexBuff);

	//使用シェーダ設定
	effect->SetTechnique("tech");
	UINT passNum = 0;
	effect->Begin(&passNum, 0);
	effect->BeginPass(0);

	//シェーダのグローバル変数を設定
	effect->SetTexture("tex", texture);
	effect->SetMatrix("mtxView", &GetMtxView());
	effect->SetMatrix("mtxProj", &GetMtxProjection());

	//インデックスバッファ設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//アルファブレンディングで描画
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//加算合成で描画
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//シェーダ終了宣言
	effect->EndPass();
	effect->End();

	//ストリーム周波数をもとに戻す
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);

	//アルファテストを戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	//描画方法をもとに戻す
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/**********************************************
パーティクルセット処理1
**********************************************/
void SetParticle(D3DXVECTOR3 pos)
{
	PARTICLE_HOGE *ptr = &particle[0];

	for (int i = 0; i < PARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->moveDir = D3DXVECTOR3(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
		ptr->speed = RandomRange(10.0f, 100.0f);
		ptr->cntFrame = 0;
		ptr->pos = pos;
		ptr->lifeFrame = rand() % 120 + 60;
		ptr->scale = RandomRange(0.5f, 10.0f);
		ptr->active = true;

		vtxColor[i].r = RandomRange(0.0f, 1.0f);
		vtxColor[i].g = RandomRange(0.0f, 1.0f);
		vtxColor[i].b = RandomRange(0.0f, 1.0f);
		cntParticle++;
		return;
	}
}

/**********************************************
パーティクルセット処理2
**********************************************/
void SetCircleParticle(D3DXVECTOR3 pos)
{
	PARTICLE_HOGE *ptr = &particle[0];

	for (int i = 0; i < PARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		D3DXVECTOR3 dir = D3DXVECTOR3(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
		D3DXVec3Normalize(&dir, &dir);

		ptr->pos = pos;

		ptr->moveDir = dir * -1;
		ptr->speed = RandomRange(0.5f, 10.0f);
		ptr->cntFrame = 0;
		ptr->lifeFrame = rand() % 60 + 60;
		ptr->scale = RandomRange(0.5f, 10.0f);
		ptr->active = true;

		vtxColor[i].r = RandomRange(0.0f, 1.0f);
		vtxColor[i].g = RandomRange(0.0f, 1.0f);
		vtxColor[i].b = RandomRange(0.0f, 1.0f);
		cntParticle++;
		return;
	}
}



