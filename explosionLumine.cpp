//=====================================
//
//爆発ルミネ処理[explosionLumine.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "explosionLumine.h"
#include "particleManager.h"
#include "Easing.h"
#include "battleCamera.h"
#include "particleFramework.h"

/**************************************
マクロ定義
***************************************/
#define EXPLOSIONLUMINE_NUM_MAX				(1024)
#define EXPLOSIONLUMINE_SIZE				(6.0f)
#define EXPLOSIONLUMINE_TEX_NAME			"data/TEXTURE/PARTICLE/explosionLumine.png"
#define EXPLOSIONLUMINE_LIFEFRAME_BASE		(60)
#define EXPLOSIONLUMINE_LIFEFRAME_RANGE		(30)
#define EXPLOSIONLUMINE_POS_RANGE			(30.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
//単位頂点
static VERTEX_PARTICLE vtx[NUM_VERTEX] = {
	{ -EXPLOSIONLUMINE_SIZE, EXPLOSIONLUMINE_SIZE, 0.0f, 0.0f, 0.0f },
	{  EXPLOSIONLUMINE_SIZE, EXPLOSIONLUMINE_SIZE, 0.0f, 1.0f, 0.0f },
	{ -EXPLOSIONLUMINE_SIZE,-EXPLOSIONLUMINE_SIZE, 0.0f, 0.0f, 1.0f },
	{  EXPLOSIONLUMINE_SIZE,-EXPLOSIONLUMINE_SIZE, 0.0f, 1.0f, 1.0f },
};

static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff, uvBuff, colorBuff;//頂点バッファ
static LPDIRECT3DTEXTURE9 texture;								//テクスチャ
static LPDIRECT3DINDEXBUFFER9 indexBuff;						//インデックスバッファ

static VERTEX_UV vtxUV[EXPLOSIONLUMINE_NUM_MAX];				//UV座標配列
static D3DXVECTOR3 pos[EXPLOSIONLUMINE_NUM_MAX];				//座標配列
static VERTEX_COLOR vtxColor[EXPLOSIONLUMINE_NUM_MAX];			//ディフューズ配列
static D3DXMATRIX mtxWorld[EXPLOSIONLUMINE_NUM_MAX];			//ワールド変換行列

static EXPLOSION_LUMINE lumine[EXPLOSIONLUMINE_NUM_MAX];		//パーティクル配列


/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitExplosionLumine(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EXPLOSION_LUMINE *ptr = &lumine[0];

	//配列初期化
	for (int i = 0; i < EXPLOSIONLUMINE_NUM_MAX; i++)
	{
		lumine[i].active = false;

		vtxUV[i].u = vtxUV[i].v = 0.0f;

		vtxColor[i].r = vtxColor[i].g = vtxColor[i].b = 1.0f;
		vtxColor[i].a = 0.0f;
	}

	if (!initialized)
	{
		//頂点バッファ作成	
		MakeParticleVertexBuffer(vtx, EXPLOSIONLUMINE_SIZE, &vtxBuff);
		MakeParticleUVBuffer(EXPLOSIONLUMINE_NUM_MAX, vtxUV, &uvBuff);
		MakeParticleWorldBuffer(EXPLOSIONLUMINE_NUM_MAX, mtxWorld, &worldBuff);
		MakeParticleColorBuffer(EXPLOSIONLUMINE_NUM_MAX, vtxColor, &colorBuff);

		//インデックスバッファ作成		
		WORD index[6] = { 0, 1, 2, 2, 1, 3 };
		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void* p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		//テクスチャ読み込み
		texture = CreateTextureFromFile((LPSTR)EXPLOSIONLUMINE_TEX_NAME, pDevice);

		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitExplosionLumine(int num)
{
	EXPLOSION_LUMINE *ptr = &lumine[0];
	VERTEX_COLOR *pColor = &vtxColor[0];
	for (int i = 0; i < EXPLOSIONLUMINE_NUM_MAX; i++, ptr++, pColor++)
	{
		ptr->active = false;
		pColor->a = 0.0f;
	}
	CopyVtxBuff(sizeof(vtxColor), vtxColor, colorBuff);

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(worldBuff);
		SAFE_RELEASE(uvBuff);
		SAFE_RELEASE(colorBuff);
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateExplosionLumine(void)
{
	EXPLOSION_LUMINE *ptr = &lumine[0];
	D3DXMATRIX mtxTranslate;
	D3DXVECTOR3 *pPos = &pos[0];
	VERTEX_COLOR *pColor = &vtxColor[0];
	D3DXMATRIX *pWorld = &mtxWorld[0];

	for (int i = 0; i < EXPLOSIONLUMINE_NUM_MAX; i++, pPos++, pColor++, pWorld++, ptr++)
	{
		if (!ptr->active)
			continue;

		ptr->cntFrame++;

		//寿命判定	
		if (ptr->cntFrame > ptr->lifeFrame)
		{
			pColor->a = 0.0f;
			ptr->active = false;
			continue;
		}

		//移動処理
		*pPos += ptr->moveDir * 5.0f;

		//ワールド変換行列更新
		D3DXMatrixIdentity(pWorld);
		GetInvRotBattleCamera(pWorld);
		D3DXMatrixTranslation(&mtxTranslate, pPos->x, pPos->y, pPos->z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxTranslate);

		//ディフューズ計算処理
		float t = (float)ptr->cntFrame / (float)ptr->lifeFrame;
		pColor->a = EaseInCubic(t, 1.0f, 0.0f);
	}

	//頂点バッファへコピー	
	CopyVtxBuff(sizeof(mtxWorld), mtxWorld, worldBuff);
	CopyVtxBuff(sizeof(vtxColor), vtxColor, colorBuff);
}

/**************************************
描画処理
***************************************/
void DrawExplosionLumine(LPD3DXEFFECT effect)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (EXPLOSIONLUMINE_NUM_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_PARTICLE));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetStreamSource(2, uvBuff, 0, sizeof(VERTEX_UV));
	pDevice->SetStreamSource(3, colorBuff, 0, sizeof(VERTEX_COLOR));

	pDevice->SetIndices(indexBuff);

	effect->SetTexture("tex", texture);
	effect->SetMatrix("mtxView", &GetBattleCameraView());
	effect->SetMatrix("mtxProj", &GetBattleCameraProjection());

	effect->SetTechnique("tech");
	UINT passNum = 0;
	effect->Begin(&passNum, 0);
	effect->BeginPass(0);

	//通常描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0, NUM_POLYGON);

	//加算描画
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0, NUM_POLYGON);

	effect->EndPass();
	effect->End();

	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

/**************************************
セット処理
***************************************/
void SetExplosionLumine(D3DXVECTOR3 *setPos)
{
	EXPLOSION_LUMINE *ptr = &lumine[0];
	D3DXVECTOR3 *pPos = &pos[0];
	VERTEX_COLOR *pColor = &vtxColor[0];

	for (int i = 0; i < EXPLOSIONLUMINE_NUM_MAX; i++, ptr++, pPos++, pColor++)
	{
		if (ptr->active)
			continue;

		*pPos = *setPos;

		pColor->a = 1.0f;

		ptr->cntFrame = 0;
		ptr->lifeFrame = EXPLOSIONLUMINE_LIFEFRAME_BASE + RandomRange(-EXPLOSIONLUMINE_LIFEFRAME_RANGE, EXPLOSIONLUMINE_LIFEFRAME_RANGE);
		ptr->moveDir = D3DXVECTOR3(RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f));
		D3DXVec3Normalize(&ptr->moveDir, &ptr->moveDir);
		ptr->active = true;
		return;
	}
}