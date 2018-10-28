//=====================================
//
//エクスプロージョンスモッグ処理[explosionSmog.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "explosionSmog.h"
#include "camera.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define EXPLOSIONSMOG_TEXNAME	"data/TEXTURE/explosionSmog.png"
#define EXPLOSIONSMOG_SIZE		(16)
#define EXPLOSIONSMOG_MAX		(128)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;
static EXPLOSIONSMOG smog[EXPLOSIONSMOG_MAX];
static D3DXMATRIX mtxWorld;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexExplosionSmog(void);
void SetExplosionSmogDiffuse(EXPLOSIONSMOG *ptr);

/**************************************
初期化処理
***************************************/
void InitExplosionSmog(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		MakeVertexExplosionSmog();

		texture = CreateTextureFromFile((LPSTR)EXPLOSIONSMOG_TEXNAME, pDevice);
	}

	EXPLOSIONSMOG *ptr = &smog[0];
	for (int i = 0; i < EXPLOSIONSMOG_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

/**************************************
終了処理
***************************************/
void UninitExplosionSmog(void)
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(vtxBuff);
}

/**************************************
更新処理
***************************************/
void UpdateExplosionSmog(void)
{
	EXPLOSIONSMOG *ptr = &smog[0];

	for (int i = 0; i < EXPLOSIONSMOG_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//ptr->scale = EaseOutCubic((float)ptr->cntFrame, 1.0f, 0.0f, (float)ptr->lifeFrame);
		ptr->alpha = EaseInOutCubic((float)ptr->cntFrame, 0.5f, 0.0f, (float)ptr->lifeFrame);
		ptr->pos += ptr->moveDir * EaseOutExponential((float)ptr->cntFrame, ptr->initSpeed, ptr->endSpeed, (float)ptr->lifeFrame);
		ptr->cntFrame++;

		if (ptr->cntFrame == ptr->lifeFrame)
		{
			ptr->active = false;
		}
	}
}

/**************************************
描画処理
***************************************/
void DrawExplosionSmog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxScale;

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, texture);


	EXPLOSIONSMOG *ptr = &smog[0];
	for (int i = 0; i < EXPLOSIONSMOG_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		GetInvCameraRotMtx(&mtxWorld);

		D3DXMatrixScaling(&mtxScale, ptr->scale, ptr->scale, ptr->scale);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		SetExplosionSmogDiffuse(ptr);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexExplosionSmog(void)
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

	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-EXPLOSIONSMOG_SIZE, EXPLOSIONSMOG_SIZE, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(EXPLOSIONSMOG_SIZE, EXPLOSIONSMOG_SIZE, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-EXPLOSIONSMOG_SIZE, -EXPLOSIONSMOG_SIZE, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(EXPLOSIONSMOG_SIZE, -EXPLOSIONSMOG_SIZE, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 1.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f ,0.5f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxBuff->Unlock();
}

/**************************************
エクスプロージョンファイアセット処理
***************************************/
void SetExplosionSmog(const D3DXVECTOR3 *pos)
{
	EXPLOSIONSMOG *ptr = &smog[0];

	for (int i = 0; i < EXPLOSIONSMOG_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = *pos;
		ptr->initSpeed = RandomRange(0.5f, 3.0f);
		ptr->endSpeed = 0.0f;
		ptr->lifeFrame = 90;
		ptr->cntFrame = 0;
		ptr->rot = RandomRange(0.0f, 360.0f);
		ptr->moveDir = D3DXVECTOR3(cosf(D3DXToRadian(ptr->rot)), sinf(D3DXToRadian(ptr->rot)), RandomRange(-1, 1));
		//ptr->moveDir = D3DXVECTOR3(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
		ptr->scale = RandomRange(1.5f, 1.8f);
		ptr->alpha = 1.0f;
		ptr->active = true;
		return;
	}
}

void SetExplosionSmogDiffuse(EXPLOSIONSMOG *ptr)
{
	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, ptr->alpha);

	vtxBuff->Unlock();
}