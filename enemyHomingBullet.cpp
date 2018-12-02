//=====================================
//
//エネミーホーミングバレット処理[enemyHomingBullette.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "enemyHomingBullet.h"
#include "battleCamera.h"
#include "playerModel.h"

/**************************************
マクロ定義
***************************************/
#define ENEMYHOMINGBULLET_TEXTURE_NAME			"data/TEXTURE/ENEMY/enemyBullet01.png"
#define ENEMYHOMINGBULLET_TEXTURE_SIZE_X		(5)
#define ENEMYHOMINGBULLET_TEXTURE_SIZE_Y		(5)
#define ENEMYHOMINGBULLET_DISABLE_BORDER_Z		(-200.0f)
#define ENEMYHOMINGBULLET_ACCELERATION_MAX		(500.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static ENEMYHOMINGBULLET bullet[ENEMYHOMINGBULLET_MAX];

/**************************************
プロトタイプ宣言
***************************************/
void CreateEnemyHomingBulletVertexBuffer(void);

/**************************************
初期化処理
***************************************/
void InitEnemyHomingBullet(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (num == 0)
	{
		texture = CreateTextureFromFile(ENEMYHOMINGBULLET_TEXTURE_NAME, pDevice);
		CreateEnemyHomingBulletVertexBuffer();
	}

	ENEMYHOMINGBULLET *ptr = &bullet[0];
	for (int i = 0; i < ENEMYHOMINGBULLET_MAX; i++, ptr++)
	{
		ptr->active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitEnemyHomingBullet(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
		SAFE_RELEASE(vtxBuff);
	}
}

/**************************************
更新処理
***************************************/
void UpdateEnemyHomingBullet(void)
{
	ENEMYHOMINGBULLET *ptr = &bullet[0];
	PLAYERMODEL *player = NULL;
	D3DXVECTOR3 diff, acceleration;
	float time;

	int cnt = 0;
	for (int i = 0; i < ENEMYHOMINGBULLET_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}
		cnt++;
		if (ptr->cntFrame > 0)
		{
			time = ptr->cntFrame / 60.0f;
			player = GetPlayerAdr(ptr->targetPlayerID);
			diff = player->pos - ptr->pos;
			acceleration = (diff - ptr->velocity * time) * 2.0f / (time * time);
			if (D3DXVec3LengthSq(&acceleration) > powf(ENEMYHOMINGBULLET_ACCELERATION_MAX, 2))
			{
				D3DXVec3Normalize(&acceleration, &acceleration);
				acceleration = acceleration * ENEMYHOMINGBULLET_ACCELERATION_MAX;
			}
			ptr->velocity += acceleration / 60.0f;

			ptr->cntFrame--;
		}
		else
		{
			ptr->pos = ptr->pos;
		}

		ptr->pos += ptr->velocity / 60.0f;


		if (ptr->pos.z < ENEMYHOMINGBULLET_DISABLE_BORDER_Z)
		{
			ptr->active = false;
		}
	}
	PrintDebugProc("homingbullet : %d\n", cnt);
}

/**************************************
描画処理
***************************************/
void DrawEnemyHomingBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxWorld;
	ENEMYHOMINGBULLET *ptr = &bullet[0];

	//set RenderState
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < ENEMYHOMINGBULLET_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//init world
		D3DXMatrixIdentity(&mtxWorld);

		//rotate
		GetInvRotBattleCamera(&mtxWorld);

		//translate
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//set world
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//draw
		pDevice->SetTexture(0, texture);
		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		//addictive draw
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/**************************************
頂点作成処理
***************************************/
void CreateEnemyHomingBulletVertexBuffer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		0)))
	{
		return;
	}

	VERTEX_3D *pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-ENEMYHOMINGBULLET_TEXTURE_SIZE_X, ENEMYHOMINGBULLET_TEXTURE_SIZE_Y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(ENEMYHOMINGBULLET_TEXTURE_SIZE_X, ENEMYHOMINGBULLET_TEXTURE_SIZE_Y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-ENEMYHOMINGBULLET_TEXTURE_SIZE_X, -ENEMYHOMINGBULLET_TEXTURE_SIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(ENEMYHOMINGBULLET_TEXTURE_SIZE_X, -ENEMYHOMINGBULLET_TEXTURE_SIZE_Y, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxBuff->Unlock();

	return;
}

/**************************************
頂点作成処理
***************************************/
void SetEnemyHomingBullet(D3DXVECTOR3 pos, D3DXVECTOR3 moveDir, float speed)
{
	ENEMYHOMINGBULLET *ptr = &bullet[0];

	for (int i = 0; i < ENEMYHOMINGBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->targetPlayerID = 0;
		ptr->cntFrame = 60;
		ptr->active = true;
		return;
	}
}