//=====================================
//
//スコア倍率GUI処理[scoreMagniGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "scoreMagniGUI.h"
#include "baseGUI.h"
#include <array>
#include <algorithm>
#include "battleCamera.h"
#include "particleFramework.h"
#include "particleManager.h"
#include "dataContainer.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define SCOREMAGNIGUI_TEX_NAME		"data/TEXTURE/UI/bonusMagni.png"
#define SCOREMAGNIGUI_SIZE_X		(0.0f)
#define SCOREMAGNIGUI_SIZE_Y		(200.0f)
#define SCOREMAGNIGUI_POS			(D3DXVECTOR3(SCOREMAGNIGUI_SIZE_X / 2.0f, 200.0f, 0.0f))
#define SCOREMAGNIGUI_MAX			(128)
#define SCOREMAGNIGUI_FADE_DURATION	(120)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff, colorBuff, uvBuff;
static LPDIRECT3DTEXTURE9 texture;
static std::array<SCOREMAGNI_GUI, SCOREMAGNIGUI_MAX> entity;

static D3DXMATRIX world[SCOREMAGNIGUI_MAX];
static VERTEX_COLOR color[SCOREMAGNIGUI_MAX];
static VERTEX_UV vtxUV[SCOREMAGNIGUI_MAX];

static VERTEX_PARTICLE vtx[NUM_VERTEX] = {
	{ -SCOREMAGNIGUI_SIZE_X,  SCOREMAGNIGUI_SIZE_Y, 0.0f, 0.0f, 0.0f },
	{  SCOREMAGNIGUI_SIZE_X,  SCOREMAGNIGUI_SIZE_Y, 0.0f, 1.0f, 0.0f },
	{ -SCOREMAGNIGUI_SIZE_X, -SCOREMAGNIGUI_SIZE_Y, 0.0f, 0.0f, 0.2f },
	{  SCOREMAGNIGUI_SIZE_X, -SCOREMAGNIGUI_SIZE_Y, 0.0f, 1.0f, 0.2f },
};

static LPDIRECT3DINDEXBUFFER9 indexBuff;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexScoreMagniGUI(void);

/**************************************
初期化処理
***************************************/
void InitScoreMagniGUI(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		texture = CreateTextureFromFile((LPSTR)SCOREMAGNIGUI_TEX_NAME, pDevice);
		MakeParticleVertexBuffer(vtx, SCOREMAGNIGUI_MAX, &vtxBuff);
		MakeParticleWorldBuffer(SCOREMAGNIGUI_MAX, world, &worldBuff);
		MakeParticleColorBuffer(SCOREMAGNIGUI_MAX, color, &colorBuff);
		MakeParticleUVBuffer(SCOREMAGNIGUI_MAX, vtxUV, &uvBuff);

		WORD index[6] = { 0, 1, 2, 2, 1, 3 };
		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void *p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		initialized = true;
	}

	for (int i = 0; i < SCOREMAGNIGUI_MAX; i++)
	{
		color[i].r = color[i].g = color[i].b = 1.0f;
		color[i].a = 0.0f;
		entity[i].active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitScoreMagniGUI(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(texture);
	}

	for (int i = 0; i < SCOREMAGNIGUI_MAX; i++)
	{
		color[i].a = 0.0f;
		entity[i].active = false;
	}
}

/**************************************
更新処理
***************************************/
void UpdateScoreMagniGUI(void)
{
	D3DXMATRIX mtxTrans;
	for (int i = 0; i < SCOREMAGNIGUI_MAX; i++)
	{
		if (!entity[i].active)
			continue;

		//透過
		entity[i].cntFrame++;
		color[i].a = 1.0f - (float)entity[i].cntFrame / (float)SCOREMAGNIGUI_FADE_DURATION;

		if (entity[i].cntFrame == SCOREMAGNIGUI_FADE_DURATION)
		{
			entity[i].active = false;
		}

		D3DXMatrixIdentity(&world[i]);
		GetInvRotBattleCamera(&world[i]);
		D3DXMatrixTranslation(&mtxTrans, entity[i].pos.x, entity[i].pos.y, entity[i].pos.z);
		D3DXMatrixMultiply(&world[i], &world[i], &mtxTrans);
	}

	//頂点バッファにメモリコピー
	CopyVtxBuff(sizeof(D3DXMATRIX) * SCOREMAGNIGUI_MAX, world, worldBuff);
	CopyVtxBuff(sizeof(VERTEX_COLOR) * SCOREMAGNIGUI_MAX, color, colorBuff);
	CopyVtxBuff(sizeof(VERTEX_UV) * SCOREMAGNIGUI_MAX, vtxUV, uvBuff);
}

/**************************************
描画処理
***************************************/
void DrawScoreMagniGUI(LPD3DXEFFECT effect)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | SCOREMAGNIGUI_MAX);
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
	effect->Begin(NULL, 0);
	effect->BeginPass(0);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
	effect->EndPass();
	effect->End();

	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);

}

/**************************************
セット処理
***************************************/
void SetScoreMagniGUI(D3DXVECTOR3 pos)
{
	for (int i = 0; i < SCOREMAGNIGUI_MAX; i++)
	{
		if (entity[i].active)
			continue;

		entity[i].pos = pos;
		
		float magni = GetScoreMagni();

		if (magni == 16.0f)
			vtxUV[i].v = 0.8f;
		else if (magni > 8.0f)
			vtxUV[i].v = 0.6f;
		else if (magni > 4.0f)
			vtxUV[i].v = 0.4f;
		else if (magni > 2.0f)
			vtxUV[i].v = 0.2f;
		else
			vtxUV[i].v = 0.0f;

		entity[i].cntFrame = 0;
		color[i].a = 1.0f;
		entity[i].active = true;

	}
}