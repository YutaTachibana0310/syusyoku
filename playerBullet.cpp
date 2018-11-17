//=====================================
//
//テンプレート処理[playerBullet.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerBullet.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERULLET_SPEED		(25.0f)
#define PLAYERBULLET_TEXNAME	"data/TEXTURE/playerbullet.png"
#define PLAYERBULLET_TEXSIZE_X	(5.0f)
#define PLAYERBULLET_TEXSIZE_Y	(5.0f)
#define PLAYERBULLET_ROTVALUE	(0.05f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static LPDIRECT3DTEXTURE9 tex;
static D3DXMATRIX mtxWorld;

static PLAYERBULLET bullet[PLAYERBULLET_MAX];

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexPlayerBullet(void);

/**************************************
初期化処理
***************************************/
void InitPlayerBullet(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (num == 0)
	{
		MakeVertexPlayerBullet();

		tex = CreateTextureFromFile((LPSTR)PLAYERBULLET_TEXNAME, pDevice);
	}

	PLAYERBULLET *ptr = &bullet[0];
	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		ptr->active = false;
		ptr->pos = D3DXVECTOR3(9999.9f, 9999.9f, 9999.9f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
}

/**************************************
終了処理
***************************************/
void UninitPlayerBullet(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(tex);
	}

	PLAYERBULLET *ptr = &bullet[0];
	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		ptr->active = false;
		
	}
}

/**************************************
更新処理
***************************************/
void UpdatePlayerBullet(void)
{
	PLAYERBULLET *ptr = &bullet[0];
	BATTLECAMERA *camera = GetBattleCameraAdr();

	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->pos += ptr->moveDir * ptr->speed;

		switch (camera->currentState)
		{
		case SideViewCamera:
			ptr->rot.x = ptr->rotation;
			ptr->rot.y = ptr->rot.z = 0.0f;
			break;
			
		case TopViewCamera:
			ptr->rot.z = 0.0f;// ptr->rotation;
			ptr->rot.y = ptr->rot.x = 0.0f;
			break;

		case FirstPersonCamera:
		case QuaterViewCamera:
			ptr->rot.z = ptr->rotation;
			ptr->rot.x = ptr->rot.y = 0.0f;
			break;

		default:
			ptr->rot.x = ptr->rot.y = ptr->rot.z = 0.0f;
			break;
		}
		

		if (ptr->pos.z > 1000.0f)
		{
			ptr->active = false;
		
		}
	}
}

/**************************************
描画処理
***************************************/
void DrawPlayerBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERBULLET *ptr = &bullet[0];
	D3DXMATRIX mtxRot, mtxScale, mtxTranslate;

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, tex);

	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		GetInvRotBattleCamera(&mtxWorld);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->rot.y, ptr->rot.x, ptr->rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexPlayerBullet(void)
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

	pVtx[0].vtx = D3DXVECTOR3(-PLAYERBULLET_TEXSIZE_X, PLAYERBULLET_TEXSIZE_Y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(PLAYERBULLET_TEXSIZE_X, PLAYERBULLET_TEXSIZE_Y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-PLAYERBULLET_TEXSIZE_X, -PLAYERBULLET_TEXSIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(PLAYERBULLET_TEXSIZE_X, -PLAYERBULLET_TEXSIZE_Y, 0.0f);

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

	return;
}

/**************************************
プレイヤーバレットセット関数
***************************************/
void SetPlayerBullet(D3DXVECTOR3 pos, float speed)
{
	PLAYERBULLET *ptr = &bullet[0];
	const float randomAngle = 0.1f;

	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->rotation = RandomRange(0.0f, 6.12f);

		ptr->moveDir = D3DXVECTOR3(RandomRange(-randomAngle, randomAngle), RandomRange(-randomAngle, randomAngle), 1.0f);
		D3DXVec3Normalize(&ptr->moveDir, &ptr->moveDir);

		ptr->speed = speed;

		ptr->active = true;
		return;
	}
}

/**************************************
プレイヤーバレットゲット関数
***************************************/
PLAYERBULLET *GetPlayerBulletAdr(int id)
{
	return &bullet[id];
}