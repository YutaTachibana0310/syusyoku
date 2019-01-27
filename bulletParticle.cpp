//=====================================
//
//バレットパーティクル処理[bulletParticle.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "particleManager.h"
#include "bulletParticle.h"
#include "particleFramework.h"
#include "battleCamera.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define BULLETPARTICLE_MAX			(1024)
#define BULLETPARTICLE_TEXNAME		"data/TEXTURE/ENEMY/enemyBulletTrail.png"
#define BULLETPARTICLE_SIZE			(4.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static D3DXMATRIX pos[BULLETPARTICLE_MAX];
static VERTEX_COLOR vtxColor[BULLETPARTICLE_MAX];
static VERTEX_UV vtxUV[BULLETPARTICLE_MAX];

//単位頂点
static VERTEX_PARTICLE vtx[NUM_VERTEX] = {
	{ -BULLETPARTICLE_SIZE, BULLETPARTICLE_SIZE, 0.0f, 0.0f, 0.0f },
	{ BULLETPARTICLE_SIZE, BULLETPARTICLE_SIZE, 0.0f, 0.5f, 0.0f },
	{ -BULLETPARTICLE_SIZE, -BULLETPARTICLE_SIZE, 0.0f, 0.0f, 1.0f },
	{ BULLETPARTICLE_SIZE, -BULLETPARTICLE_SIZE, 0.0f, 0.5f, 1.0f }
};

//各種頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static LPDIRECT3DVERTEXBUFFER9 uvBuff;
static LPDIRECT3DVERTEXBUFFER9 posBuff;
static LPDIRECT3DVERTEXBUFFER9 colorBuff;

static LPDIRECT3DTEXTURE9 texture;
static LPDIRECT3DINDEXBUFFER9 indexBuff;
static BULLETPARTICLE particle[BULLETPARTICLE_MAX];

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitBulletParticle(int num)
{
	static bool initialized = false;

	//配列初期化
	for (int i = 0; i < BULLETPARTICLE_MAX; i++)
	{
		vtxColor[i].r = vtxColor[i].g = vtxColor[i].b = 1.0f;
		vtxColor[i].a = 0.0f;
		particle[i].active = false;
	}

	//初回のみ初期化
	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		//頂点バッファ作成
		MakeParticleVertexBuffer(vtx, BULLETPARTICLE_SIZE, &vtxBuff);
		MakeParticleUVBuffer(BULLETPARTICLE_MAX, vtxUV, &uvBuff);
		MakeParticleWorldBuffer(BULLETPARTICLE_MAX, pos, &posBuff);
		MakeParticleColorBuffer(BULLETPARTICLE_MAX, vtxColor, &colorBuff);

		//インデックスバッファ作成
		WORD index[6] = { 0, 1, 2, 2, 1, 3 };
		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void *p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		//テクスチャ読み込み
		texture = CreateTextureFromFile((LPSTR)BULLETPARTICLE_TEXNAME, pDevice);

		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitBulletParticle(int num)
{
	for (int i = 0; i < BULLETPARTICLE_MAX; i++)
	{
		particle[i].active = false;
		vtxColor[i].a = 0.0f;
	}
	CopyVtxBuff(sizeof(vtxColor), vtxColor, colorBuff);

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(uvBuff);
		SAFE_RELEASE(posBuff);
		SAFE_RELEASE(colorBuff);
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateBulletParticle(void)
{
	BULLETPARTICLE *ptr = &particle[0];
	D3DXMATRIX mtxTranslate, mtxScale;

	//パーティクルの更新
	for (int i = 0; i < BULLETPARTICLE_MAX; i++, ptr++)
	{
		if (!ptr->active)
			continue;

		//移動
		float t = (float)ptr->cntFrame / (float)ptr->lifeFrame;
		float speed = GetEasingValue(t, ptr->initSpeed, ptr->endSpeed, OutCubic);
		ptr->pos += speed * ptr->moveDir;
		ptr->pos.z += PARTICLE_SCROLLSPEED;

		//透過
		vtxColor[i].a = 1.0f;// EaseLinear(t, 1.0f, 0.0f);

		ptr->cntFrame++;

		//寿命判定
		if (ptr->cntFrame == ptr->lifeFrame)
		{
			vtxColor[i].a = 0.0f;
			ptr->active = false;
		}

		//ワールド変換行列更新
		D3DXMatrixIdentity(&pos[i]);
		GetInvRotBattleCamera(&pos[i]);
		D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
		D3DXMatrixMultiply(&pos[i], &pos[i], &mtxScale);
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&pos[i], &pos[i], &mtxTranslate);
	}

	//頂点バッファにメモリコピー
	CopyVtxBuff(sizeof(D3DXMATRIX) * BULLETPARTICLE_MAX, pos, posBuff);
	CopyVtxBuff(sizeof(VERTEX_COLOR) * BULLETPARTICLE_MAX, vtxColor, colorBuff);
	CopyVtxBuff(sizeof(VERTEX_UV) * BULLETPARTICLE_MAX, vtxUV, uvBuff);
}

/**************************************
描画処理
***************************************/
void DrawBulletParticle(LPDIRECT3DVERTEXDECLARATION9 declare, LPD3DXEFFECT effect)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//αテスト使用
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//ストリーム周波数を設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (BULLETPARTICLE_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//ストリームソース設定
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_PARTICLE));
	pDevice->SetStreamSource(1, posBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetStreamSource(2, uvBuff, 0, sizeof(VERTEX_UV));
	pDevice->SetStreamSource(3, colorBuff, 0, sizeof(VERTEX_COLOR));
	pDevice->SetIndices(indexBuff);

	//シェーダのグローバル変数を設定
	effect->SetTexture("tex", texture);
	effect->SetMatrix("mtxView", &GetBattleCameraView());
	effect->SetMatrix("mtxProj", &GetBattleCameraProjection());

	//使用シェーダ設定
	effect->SetTechnique("tech");
	UINT passNum = 0;
	effect->Begin(&passNum, 0);
	effect->BeginPass(0);

	//インデックスバッファ設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//アルファブレンディングで描画
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//加算合成で描画
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

/**************************************
セット処理
***************************************/
void SetBulletParticle(const D3DXVECTOR3 *setPos, BULLETEXPL_TYPE type)
{
	BULLETPARTICLE *ptr = &particle[0];
	for (int i = 0; i < BULLETPARTICLE_MAX; i++, ptr++)
	{
		if (ptr->active)
			continue;

		ptr->active = true;

		//フレーム関連の設定
		ptr->cntFrame = 0;
		ptr->lifeFrame = 30;

		//スピードの設定
		ptr->initSpeed = RandomRangef(5.0f, 10.0f);
		ptr->endSpeed = 1.0f;

		//座標の設定
		ptr->pos = *setPos;
		ptr->moveDir = D3DXVECTOR3(RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f), RandomRangef(0.0f, 1.0f));
		D3DXVec3Normalize(&ptr->moveDir, &ptr->moveDir);

		vtxUV[i].u = type * 1.0f / BULLETEXPL_TYPEMAX;
		return;
	}
}

/**************************************
放出処理
***************************************/
void EmmittBulletParticle(const D3DXVECTOR3 *setPos, BULLETEXPL_TYPE type, int num)
{
	for (int i = 0; i < num; i++)
	{
		SetBulletParticle(setPos, type);
	}
}