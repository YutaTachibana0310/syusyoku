//=====================================
//
//エクスプロージョンフレア処理[explosionFire.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "explosionFlare.h"
#include "camera.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define EXPLOSIONFLARE_TEXNAME	"data/TEXTURE/explosionFlare.png"
#define EXPLOSIONFLARE_SIZE		(10)
#define EXPLOSIONFLARE_MAX		(2048)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;
static EXPLOSIONFLARE smog[EXPLOSIONFLARE_MAX];
static D3DXMATRIX mtxWorld;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexExplosionFlare(void);
void SetExplosionFlareDiffuse(EXPLOSIONFLARE *ptr, VERTEX_3D *pVtx);

/**************************************
初期化処理
***************************************/
void InitExplosionFlare(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		MakeVertexExplosionFlare();

		texture = CreateTextureFromFile((LPSTR)EXPLOSIONFLARE_TEXNAME, pDevice);
	}

	EXPLOSIONFLARE *ptr = &smog[0];
	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

/**************************************
終了処理
***************************************/
void UninitExplosionFlare(void)
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(vtxBuff);
}

/**************************************
更新処理
***************************************/
void UpdateExplosionFlare(void)
{
	EXPLOSIONFLARE *ptr = &smog[0];

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		if (ptr->cntFrame == ptr->lifeFrame)
		{
			ptr->active = false;
			continue;
		}

		ptr->scale = EaseOutCubic((float)ptr->cntFrame, ptr->initScale, 0.0f, (float)ptr->lifeFrame);
		ptr->alpha = EaseOutCubic((float)ptr->cntFrame, 1.0f, 0.0f, (float)ptr->lifeFrame);
		ptr->pos += ptr->moveDir * EaseOutExponential((float)ptr->cntFrame, ptr->initSpeed, ptr->endSpeed, (float)ptr->lifeFrame);
		ptr->cntFrame++;
	}
}

/**************************************
描画処理
***************************************/
void DrawExplosionFlare(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxScale, mtxRot;
	D3DXQUATERNION rotQ;

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, texture);

	VERTEX_3D *pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

	EXPLOSIONFLARE *ptr = &smog[0];
	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		D3DXQuaternionRotationAxis(&rotQ, &(GetCameraPos() - ptr->pos), ptr->rot);
		D3DXMatrixRotationQuaternion(&mtxRot, &rotQ);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		GetInvCameraRotMtx(&mtxWorld);
			
		D3DXMatrixScaling(&mtxScale, ptr->scale, ptr->scale, ptr->scale);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		SetExplosionFlareDiffuse(ptr, pVtx);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	ptr = &smog[0];
	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
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

		SetExplosionFlareDiffuse(ptr, pVtx);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLEND_INVSRCALPHA);

	vtxBuff->Unlock();
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexExplosionFlare(void)
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

	pVtx[0].vtx = D3DXVECTOR3(-EXPLOSIONFLARE_SIZE, EXPLOSIONFLARE_SIZE, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(EXPLOSIONFLARE_SIZE, EXPLOSIONFLARE_SIZE, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-EXPLOSIONFLARE_SIZE, -EXPLOSIONFLARE_SIZE, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(EXPLOSIONFLARE_SIZE, -EXPLOSIONFLARE_SIZE, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 1.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxBuff->Unlock();
}

/**************************************
ディフューズ設定処理
***************************************/
void SetExplosionFlareDiffuse(EXPLOSIONFLARE *ptr, VERTEX_3D *pVtx)
{
	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, ptr->alpha);
}

/**************************************
エクスプロージョンフレアセット処理
***************************************/
void SetExplosionFlare(const D3DXVECTOR3 *pos)
{
	EXPLOSIONFLARE *ptr = &smog[0];

	for (int i = 0; i < EXPLOSIONFLARE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = *pos;		
		ptr->initSpeed = RandomRange(1.0f, 20.0f);
		ptr->endSpeed = 0.0f;
		ptr->lifeFrame = 60;
		ptr->cntFrame = 0;
		ptr->rot = RandomRange(0.0f, 360.0f);
		ptr->moveDir = D3DXVECTOR3(cosf(D3DXToRadian(ptr->rot)), sinf(D3DXToRadian(ptr->rot)), RandomRange(-1, 1));
		//ptr->moveDir = D3DXVECTOR3(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
		ptr->initScale = ptr->scale = RandomRange(0.1f, 1.3f);
		ptr->alpha = 1.0f;
		ptr->active = true;
		return;
	}
}