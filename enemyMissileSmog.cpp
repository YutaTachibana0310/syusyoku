//=====================================
//
//エネミーミサイルスモッグ処理[enemyMissileSmog.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "enemyMissileSmog.h"
#include "camera.h"

/**************************************
マクロ定義
***************************************/
#define ENEMYMISSILESMOG_MAX		(512)
#define ENEMYMISSILESMOG_SIZE		(8)
#define ENEMYMISSILESMOG_TEXNAME	"data/TEXTURE/missileSmog.png"
#define ENEMYMISSILESMOG_LIFEFRAME	(120)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static LPDIRECT3DTEXTURE9 texture;
static D3DXMATRIX mtxWorld;

static EnemyMissileSmog smog[ENEMYMISSILESMOG_MAX];

/**************************************
プロトタイプ宣言
***************************************/
void SetVertexBufferEnemyMissileSmog(void);
void SetDiffuseEnemyMissileSmog(EnemyMissileSmog *ptr, VERTEX_3D *pVtx);

/**************************************
初期化処理
***************************************/
void InitEnemyMissileSmog(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		SetVertexBufferEnemyMissileSmog();

		texture = CreateTextureFromFile((LPSTR)ENEMYMISSILESMOG_TEXNAME, pDevice);
	}

	EnemyMissileSmog *ptr = &smog[0];
	for (int i = 0; i < ENEMYMISSILESMOG_MAX; i++, ptr++)
	{
		ptr->active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitEnemyMissileSmog(void)
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(vtxBuff);
}

/**************************************
更新処理
***************************************/
void UpdateEnemyMissileSmog(void)
{
	EnemyMissileSmog *ptr = &smog[0];

	for (int i = 0; i < ENEMYMISSILESMOG_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->cntFrame++;

		if (ptr->cntFrame == ENEMYMISSILESMOG_LIFEFRAME)
		{
			ptr->active = false;
		}
	}
}

/**************************************
描画処理
***************************************/
void DrawEnemyMissileSmog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;
	VERTEX_3D *pVtx = NULL;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, texture);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	EnemyMissileSmog *ptr = &smog[0];

	for (int i = 0; i < ENEMYMISSILESMOG_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		GetInvCameraRotMtx(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		SetDiffuseEnemyMissileSmog(ptr, pVtx);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	vtxBuff->Unlock();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

/**************************************
頂点作成処理
***************************************/
void SetVertexBufferEnemyMissileSmog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL)))
	{
		return;
	}

	VERTEX_3D *pVtx = NULL;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-ENEMYMISSILESMOG_SIZE, ENEMYMISSILESMOG_SIZE, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(ENEMYMISSILESMOG_SIZE, ENEMYMISSILESMOG_SIZE, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-ENEMYMISSILESMOG_SIZE, -ENEMYMISSILESMOG_SIZE, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(ENEMYMISSILESMOG_SIZE, -ENEMYMISSILESMOG_SIZE, 0.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].nor =
		pVtx[0].nor =
		pVtx[0].nor =
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, -1.0f);

	vtxBuff->Unlock();
}

/**************************************
ディフーズ設定処理
***************************************/
void SetDiffuseEnemyMissileSmog(EnemyMissileSmog *ptr, VERTEX_3D *pVtx)
{
	
	float alpha = 1.0f - (float)ptr->cntFrame / ENEMYMISSILESMOG_LIFEFRAME;
	alpha = Clampf(0.0f, 1.0f, alpha);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}

/**************************************
スモッグ設定処理
***************************************/
void SetEnemyMissileSmog(D3DXVECTOR3 pos)
{
	EnemyMissileSmog *ptr = &smog[0];

	for (int i = 0; i < ENEMYMISSILESMOG_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->cntFrame = 0;
		ptr->active = true;
		return;
	}
}