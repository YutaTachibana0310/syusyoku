//=====================================
//
//エネミーバレット処理[enemyBullette.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "enemyBullet.h"
#include "battleCamera.h"
#include "enemyBulletTrail.h"
#include "collisionManager.h"

/**************************************
マクロ定義
***************************************/
#define ENEMYBULLET_TEXTURE_NAME		"data/TEXTURE/ENEMY/enemyBullet00.png"
#define ENEMYBULLET_TEXTURE_SIZE_X		(5)
#define ENEMYBULLET_TEXTURE_SIZE_Y		(5)
#define ENEMYBULLET_DISABLE_BORDER_Z	(-200.0f)
#define ENEMYBULLET_COLLIDER_LENGTH		(2.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static ENEMYBULLET bullet[ENEMYBULLET_MAX];
static OBJECT_FOR_TREE objectForTree[ENEMYBULLET_MAX];

/**************************************
プロトタイプ宣言
***************************************/
void CreateEnemyBulletVertexBuffer(void);

/**************************************
初期化処理
***************************************/
void InitEnemyBullet(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (!initialized)
	{
		texture = CreateTextureFromFile((LPSTR)ENEMYBULLET_TEXTURE_NAME, pDevice);
		CreateEnemyBulletVertexBuffer();
		initialized = true;
	}

	ENEMYBULLET *ptr = &bullet[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < ENEMYBULLET_MAX; i++, ptr++, oft++)
	{
		ptr->collider.pos = &ptr->pos;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->collider.length = D3DXVECTOR3(ENEMYBULLET_COLLIDER_LENGTH, ENEMYBULLET_COLLIDER_LENGTH, ENEMYBULLET_COLLIDER_LENGTH);
		CreateOFT(oft, (void*)ptr);
		ptr->active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitEnemyBullet(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
		SAFE_RELEASE(vtxBuff);
	}

	ENEMYBULLET *ptr = &bullet[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < ENEMYBULLET_MAX; i++, ptr++, oft++)
	{
		ptr->active = false;
		RemoveObjectFromSpace(oft);
	}
}

/**************************************
更新処理
***************************************/
void UpdateEnemyBullet(void)
{
	ENEMYBULLET *ptr = &bullet[0];
	_OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < ENEMYBULLET_MAX; i++, ptr++, oft++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//トレイル設定
		SetEnemyHomingBulletTrail(ptr->pos, NormalEnemyBulletTrail);

		//座標更新
		ptr->pos += ptr->moveDir * ptr->speed;

		//境界判定
		if (ptr->pos.z < ENEMYBULLET_DISABLE_BORDER_Z)
		{
			ptr->active = false;
		}

		//衝突空間への登録更新
		RemoveObjectFromSpace(oft);
		if (ptr->active)
			RegisterObjectToSpace(&ptr->collider, oft, OFT_ENEMYBULLET);
	}
}

/**************************************
描画処理
***************************************/
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxWorld;
	ENEMYBULLET *ptr = &bullet[0];

	//set RenderState
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
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
void CreateEnemyBulletVertexBuffer(void)
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

	pVtx[0].vtx = D3DXVECTOR3(-ENEMYBULLET_TEXTURE_SIZE_X, ENEMYBULLET_TEXTURE_SIZE_Y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(ENEMYBULLET_TEXTURE_SIZE_X, ENEMYBULLET_TEXTURE_SIZE_Y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-ENEMYBULLET_TEXTURE_SIZE_X, -ENEMYBULLET_TEXTURE_SIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(ENEMYBULLET_TEXTURE_SIZE_X, -ENEMYBULLET_TEXTURE_SIZE_Y, 0.0f);

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
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 moveDir, float speed)
{
	ENEMYBULLET *ptr = &bullet[0];

	for (int i = 0; i < ENEMYBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		D3DXVec3Normalize(&ptr->moveDir, &moveDir);
		ptr->speed = speed;
		ptr->active = true;
		return;
	}
}