//=====================================
//
//シャドウ処理[shadow.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "shadow.h"
#include "particleManager.h"
#include "particleFramework.h"

/**************************************
マクロ定義
***************************************/
#define SHADOW_TEXTURE_NAME			"data/TEXTURE/EFFECT/shadow000.png"
#define SHADOW_SIZE_X				(20.0f)
#define SHADOW_SIZE_Z				(20.0f)
#define SHADOW_MAX					(2048)

/**************************************
構造体定義
***************************************/
typedef struct {
	bool active;
	D3DXVECTOR3 pos;

}SHADOW;
/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff, colorBuff, uvBuff;
static LPDIRECT3DTEXTURE9 texture;
static LPDIRECT3DINDEXBUFFER9 indexBuff;

static D3DXMATRIX world[SHADOW_MAX];
static VERTEX_COLOR color[SHADOW_MAX];
static VERTEX_UV vtxUv[SHADOW_MAX];

static VERTEX_PARTICLE vtx[NUM_VERTEX] = {
	{ -SHADOW_SIZE_X, 0.0f,  SHADOW_SIZE_Z, 0.0f, 0.0f },
	{  SHADOW_SIZE_X, 0.0f,  SHADOW_SIZE_Z, 1.0f, 0.0f },
	{ -SHADOW_SIZE_X, 0.0f, -SHADOW_SIZE_Z, 0.0f, 1.0f },
	{  SHADOW_SIZE_X, 0.0f, -SHADOW_SIZE_Z, 1.0f, 1.0f },
};

static SHADOW shadow[SHADOW_MAX];
static int cntShadow;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitShadow(int num)
{
	static bool initialized = false;

	//初回のみ初期化
	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		texture = CreateTextureFromFile((LPSTR)SHADOW_TEXTURE_NAME, pDevice);
		MakeParticleVertexBuffer(vtx, SHADOW_MAX, &vtxBuff);
		MakeParticleWorldBuffer(SHADOW_MAX, world, &worldBuff);
		MakeParticleColorBuffer(SHADOW_MAX, color, &colorBuff);
		MakeParticleUVBuffer(SHADOW_MAX, vtxUv, &uvBuff);

		WORD index[6] = { 0, 1, 2, 2, 1, 3 };
		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void *p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		initialized = true;
	}

	//すべての影を非アクティブに設定
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		shadow[i].active = false;
		color[i].a = 1.0f;
		color[i].r = color[i].g = color[i].b = 1.0f;
	}
	cntShadow = 0;
}

/**************************************
終了処理
***************************************/
void UninitShadow(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(texture);
		SAFE_RELEASE(uvBuff);
		SAFE_RELEASE(colorBuff);
		SAFE_RELEASE(worldBuff);
	}

	for (int i = 0; i < SHADOW_MAX; i++)
	{
		color[i].a = 0.0f;
		shadow[i].active = false;
	}
	cntShadow = 0;
}

/**************************************
更新処理
***************************************/
void UpdateShadow(void)
{
	for (int i = 0; i < SHADOW_MAX; i++)
	{
		D3DXMATRIX	mtxTranslate;
		D3DXMatrixIdentity(&world[i]);
		D3DXMatrixTranslation(&mtxTranslate, shadow[i].pos.x, shadow[i].pos.y, shadow[i].pos.z);
		D3DXMatrixMultiply(&world[i], &world[i], &mtxTranslate);
		color[i].a = 1.0f;
		shadow[i].active = false;
	}

	//頂点バッファにメモリコピー
	CopyVtxBuff(sizeof(D3DXMATRIX) * SHADOW_MAX, world, worldBuff);
	CopyVtxBuff(sizeof(VERTEX_COLOR) * SHADOW_MAX, color, colorBuff);
	CopyVtxBuff(sizeof(VERTEX_UV) * SHADOW_MAX, vtxUv, uvBuff);
}

/**************************************
描画処理
***************************************/
void DrawShadow(LPD3DXEFFECT effect)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | cntShadow);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_PARTICLE));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetStreamSource(2, uvBuff, 0, sizeof(VERTEX_COLOR));
	pDevice->SetStreamSource(3, colorBuff, 0, sizeof(VERTEX_COLOR));

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetIndices(indexBuff);

	effect->SetTexture("tex", texture);

	effect->Begin(NULL, 0);
	effect->BeginPass(0);


	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0, NUM_POLYGON);

	effect->EndPass();
	effect->End();

	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	cntShadow = 0;
}

/**************************************
シャドウセット処理
***************************************/
void SetShadow(D3DVECTOR pos)
{
	if (cntShadow == SHADOW_MAX)
		return;

	SHADOW *ptr = &shadow[cntShadow];
	ptr->active = true;
	ptr->pos = pos;
	ptr->pos.y = -500.0f;

	cntShadow++;
}