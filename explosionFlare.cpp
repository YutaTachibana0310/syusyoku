//=====================================
//
//エクスプロージョンフレア処理[explosionFlare.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "particleManager.h"
#include "battleCamera.h"
#include "debugproc.h"
#include "particleFramework.h"

#include "debugWindow.h"

/**********************************************
マクロ定義
**********************************************/
#define EXPLOSIONFLARE_MAX (2048)
#define EXPLOSIONFLARE_TEXNAME "data/TEXTURE/EFFECT/explosionFlare.png"
#define EXPLOSIONFLARE_SIZE (4)

/**********************************************
グローバル変数
**********************************************/
static D3DXMATRIX pos[EXPLOSIONFLARE_MAX];				//ワールド変換行列の配列
static VERTEX_COLOR vtxColor[EXPLOSIONFLARE_MAX];		//頂点ディフューズの配列
static VERTEX_UV vtxUV[EXPLOSIONFLARE_MAX];				//テクスチャ座標の配列

//単位パーティクルの各頂点設定
static VERTEX_PARTICLE vtx[4] = {
	{-EXPLOSIONFLARE_SIZE, EXPLOSIONFLARE_SIZE, 0.0f, 0.0f, 0.0f},
	{ EXPLOSIONFLARE_SIZE, EXPLOSIONFLARE_SIZE, 0.0f, 1.0f, 0.0f },
	{ -EXPLOSIONFLARE_SIZE, -EXPLOSIONFLARE_SIZE, 0.0f,  0.0f, 1.0f },
	{ EXPLOSIONFLARE_SIZE, -EXPLOSIONFLARE_SIZE, 0.0f, 1.0f, 1.0f }
};

//各種の頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 uvBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 posBuff = NULL;
static LPDIRECT3DVERTEXBUFFER9 colorBuff = NULL;

static LPDIRECT3DTEXTURE9 texture;					//テクスチャ
static LPDIRECT3DINDEXBUFFER9 indexBuff;			//インデックスバッファ
static PARTICLE flare[EXPLOSIONFLARE_MAX];			//パーティクル構造体

static LARGE_INTEGER startMove, endMove;
static LARGE_INTEGER startFade, endFade;
static LARGE_INTEGER startMtx, endMtx;
static LARGE_INTEGER startCopy, endCopy;

/**********************************************
プロトタイプ宣言
**********************************************/
void MoveExplosionFlare(void);
void FadeExplosionFlare(void);
void CalcMtxExplosionFlare(void);
void CheckDestroyExplosionFlare(void);

void DrawDebugWindowExplosionFlare(void);

/**********************************************
初期化処理
**********************************************/
void InitExplosionFlare(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static bool initialized = false;

	//配列初期化
	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++)
	{
		flare[i].pos = D3DXVECTOR3(-99999, -99999, -99999);
		D3DXMatrixIdentity(&pos[i]);

		vtxUV[i].u = vtxUV[i].v = 0.0f;

		vtxColor[i].r = vtxColor[i].g = vtxColor[i].g = 1.0f;
		vtxColor[i].a = 0.0f;

		flare[i].active = false;
	}

	if (!initialized)
	{	//頂点バッファ作成
	//pDevice->CreateV
		MakeParticleVertexBuffer(vtx, EXPLOSIONFLARE_SIZE, &vtxBuff);
		MakeParticleUVBuffer(EXPLOSIONFLARE_MAX, vtxUV, &uvBuff);
		MakeParticleWorldBuffer(EXPLOSIONFLARE_MAX, pos, &posBuff);
		MakeParticleColorBuffer(EXPLOSIONFLARE_MAX, vtxColor, &colorBuff);

		//インデックスバッファ作成
		WORD index[6] = { 0, 1, 2, 2, 1, 3 };
		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void *p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		//テクスチャ読み込み
		texture = CreateTextureFromFile((LPSTR)EXPLOSIONFLARE_TEXNAME, pDevice);

		initialized = true;
	}
}

/**********************************************
終了処理
**********************************************/
void UninitExplosionFlare(int num)
{
	PARTICLE *ptr = &flare[0];
	D3DXMATRIX mtxScale, mtxTranslate;

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		ptr->pos.z = -10000.0f;
		ptr->active = false;

		D3DXMatrixIdentity(&pos[i]);
		GetInvRotBattleCamera(&pos[i]);
		D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
		D3DXMatrixMultiply(&pos[i], &pos[i], &mtxScale);
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&pos[i], &pos[i], &mtxTranslate);
	}
	CopyVtxBuff(sizeof(D3DXMATRIX) * EXPLOSIONFLARE_MAX, pos, posBuff);

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(uvBuff);
		SAFE_RELEASE(posBuff);
		SAFE_RELEASE(colorBuff);
		SAFE_RELEASE(texture);
	}
}

/**********************************************
更新処理
**********************************************/
void UpdateExplosionFlare(void)
{
	GetTimerCount(&startMove);
	MoveExplosionFlare();
	GetTimerCount(&endMove);

	GetTimerCount(&startFade);
	FadeExplosionFlare();
	GetTimerCount(&endFade);

	CheckDestroyExplosionFlare();

	GetTimerCount(&startMtx);
	CalcMtxExplosionFlare();
	GetTimerCount(&endMtx);

	GetTimerCount(&startCopy);
	//頂点バッファにメモリコピー
	CopyVtxBuff(sizeof(D3DXMATRIX) * EXPLOSIONFLARE_MAX, pos, posBuff);
	CopyVtxBuff(sizeof(VERTEX_COLOR) * EXPLOSIONFLARE_MAX, vtxColor, colorBuff);
	GetTimerCount(&endCopy);

	DrawDebugWindowExplosionFlare();
}

/**********************************************
描画処理
**********************************************/
void DrawExplosionFlare(LPDIRECT3DVERTEXDECLARATION9 declare, LPD3DXEFFECT effect)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//αテスト使用
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//ストリーム周波数を設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (EXPLOSIONFLARE_MAX));
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


/**********************************************
パーティクルセット処理1
**********************************************/
bool SetExplosionFlare(const D3DXVECTOR3 *pos)
{
	PARTICLE *ptr = &flare[0];

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->active = true;

		//フレーム関連の設定
		ptr->cntFrame = 0;
		ptr->lifeFrame = (int)RandomRangef(30, 70);

		//スピードの設定
		ptr->initSpeed = RandomRangef(4.0f, 8.0f);
		ptr->endSpeed = 0.0f;
		ptr->speedType = OutExponential;

		//スケールの設定
		ptr->initScale = RandomRangef(0.8f, 1.4f);
		ptr->endScale = 1.0f;
		ptr->scaleType = Linear;

		//カラーの設定
		ptr->initRed = ptr->initGreen = ptr->initBlue = 1.0f;
		ptr->initAlpha = 0.5f;
		ptr->endRed = ptr->endGreen = ptr->endBlue = 1.0f;
		ptr->endAlpha = 0.0f;
		ptr->colorType = InCubic;

		//座標の設定
		ptr->pos = *pos;
		ptr->moveDir = D3DXVECTOR3(RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f), RandomRangef(-1.0f, 1.0f));
		D3DXVec3Normalize(&ptr->moveDir, &ptr->moveDir);

		return true;
	}

	return false;
}

/**********************************************
移動処理
**********************************************/
void MoveExplosionFlare(void)
{
	PARTICLE *ptr = &flare[0];
	float t = 0.0f;

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->cntFrame++;

		t = (float)ptr->cntFrame / (float)ptr->lifeFrame;
		ptr->pos += ptr->moveDir * GetEasingValue(t, ptr->initSpeed, ptr->endSpeed, ptr->speedType);
		ptr->pos.z += PARTICLE_SCROLLSPEED;
	}
}

/**********************************************
透過処理
**********************************************/
void FadeExplosionFlare(void)
{
	PARTICLE *ptr = &flare[0];
	float t = 0.0f;

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		t = (float)ptr->cntFrame / (float)ptr->lifeFrame;
		vtxColor[i].a = GetEasingValue(t, ptr->initAlpha, ptr->endAlpha, ptr->colorType);
	}
}

/**********************************************
マトリクス計算処理
**********************************************/
void CalcMtxExplosionFlare(void)
{
	PARTICLE *ptr = &flare[0];
	D3DXMATRIX mtxTranslate, mtxScale;
	D3DXMATRIX *pPos = &pos[0];

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, pPos++, ptr++)
	{	
		if (!ptr->active)
		{
			continue;
		}

		//座標に応じたワールド変換行列にpos配列を更新
		D3DXMatrixIdentity(pPos);
		GetInvRotBattleCamera(pPos);
		D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
		D3DXMatrixMultiply(pPos, pPos, &mtxScale);
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(pPos, pPos, &mtxTranslate);
	}
}

/**********************************************
寿命判定
**********************************************/
void CheckDestroyExplosionFlare(void)
{
	PARTICLE *ptr = &flare[0];
	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{	
		if (!ptr->active)
		{
			continue;
		}

		//寿命が来ていたら見えなくする
		if (ptr->cntFrame == ptr->lifeFrame)
		{
			ptr->pos.z = -10000.0f;
			ptr->active = false;
		}
	}
}

/**********************************************
デバッグウィンドウ表示
**********************************************/
void DrawDebugWindowExplosionFlare(void)
{

}