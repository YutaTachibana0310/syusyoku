//=====================================
//
//テンプレート処理[playerBullet.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerBullet.h"
#include "battleCamera.h"
#include "particleFramework.h"
#include "playerBulletTrail.h"
#include "dataContainer.h"
#include "collisionManager.h"
#include "particleFramework.h"

#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERULLET_SPEED			(25.0f)
#define PLAYERBULLET_TEXNAME		"data/TEXTURE/PLAYER/playerbullet.png"
#define PLAYERBULLET_TEXSIZE_X		(10.0f)
#define PLAYERBULLET_TEXSIZE_Y		(10.0f)
#define PLAYERBULLET_ROTVALUE		(0.05f)
#define PLAYERbULLET_MOVE_END		(120)
#define PLAYERBULLET_SHADER_NAME	"data/EFFECT/playerBullet.fx"
#define PLAYERBULLET_TRAIL_OFFSET	(-5.0f)
#define PLAYERBULLET_TRAIL_ALPHA	(0.2f)
#define PLAYERBULLET_TRAIL_NUM		(3)

/**************************************
構造体定義
***************************************/
enum PLAYERBULLET_STATE
{
	PlayerBulletFPS,
	PlayerBulletTop,
	PlayerBulletSide,
	PlayerBulletQuater,
	PlayerBulletStateMax
};

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 tex;
static D3DXMATRIX mtxWorld;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;

static PLAYERBULLET bullet[PLAYERBULLET_MAX];
static int currentState = 0;
static D3DXMATRIX pos;

static const D3DXVECTOR3 rotAngle[PlayerBulletStateMax] = {
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		//FPSビュー時の回転角度
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		//サイドビュー時の回転角度
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		//クォータービュー時の回転角度
};

static OBJECT_FOR_TREE objectForTree[PLAYERBULLET_MAX]; //衝突判定用のOBJECT_FOT_TREE配列

/**************************************
プロトタイプ宣言
***************************************/
void CreatePlayerBulletVertexBuffer(void);

void SetPlayerBulletLv0(D3DXVECTOR3 pos, float speed);
void SetPlayerBulletLv1(D3DXVECTOR3 pos, float speed);
void SetPlayerBulletLv2(D3DXVECTOR3 pos, float speed);
void SetPlayerBulletLv3(D3DXVECTOR3 pos, float speed);
void SetPlayerBulletLv4(D3DXVECTOR3 pos, float speed);

/**************************************
初期化処理
***************************************/
void InitPlayerBullet(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (!initialized)
	{
		//CreatePlayerBulletVertexBufferFPS();
		//CreatePlayerBulletVertexBufferSide();
		//CreatePlayerBulletVertexBufferTop();
		//CreatePlayerBulletVertexBufferQuater();
		CreatePlayerBulletVertexBuffer();

		tex = CreateTextureFromFile((LPSTR)PLAYERBULLET_TEXNAME, pDevice);

		initialized = true;
	}

	PLAYERBULLET *ptr = &bullet[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++, oft++)
	{
		ptr->active = false;
		ptr->id = i;

		ptr->pos = D3DXVECTOR3(9999.9f, 9999.9f, 9999.9f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		ptr->collider.pos = &ptr->pos;
		ptr->collider.radius = PLAYERBULLET_COLLIDER_RAIDUS;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->collider2.pos = &ptr->pos;
		ptr->collider2.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->collider2.length = D3DXVECTOR3(PLAYERBULLET_COLLIDER_RAIDUS, PLAYERBULLET_COLLIDER_RAIDUS, PLAYERBULLET_COLLIDER_RAIDUS);

		ptr->destroyRequest = false;

		CreateOFT(oft, (void*)ptr);
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
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++, oft++)
	{
		ptr->active = false;
		RemoveObjectFromSpace(oft);
	}
}

/**************************************
更新処理
***************************************/
void UpdatePlayerBullet(void)
{
	PLAYERBULLET *ptr = &bullet[0];
	BATTLECAMERA *camera = GetBattleCameraAdr();
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	int cntActive = 0;
	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++, oft++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//消滅判定
		if (ptr->destroyRequest)
		{
			ptr->active = false;
			ptr->destroyRequest = false;
			RemoveObjectFromSpace(oft);
			continue;
		}

		cntActive++;
		//座標更新
		ptr->pos += ptr->moveDir * ptr->speed;

		//トレイルセット
		for (int j = 0; j < PLAYERBULLET_TRAIL_NUM; j++)
		{
			SetPlayerBulletTrail(ptr->pos + ptr->moveDir * PLAYERBULLET_TRAIL_OFFSET * j, 1.0f - PLAYERBULLET_TRAIL_ALPHA * j );
		}

		//境界判定
		if (ptr->pos.z > 1000.0f)
		{
			ptr->active = false;
			RemoveObjectFromSpace(oft);
			continue;
		}

		//空間登録更新
		D3DXVECTOR3 pos = ptr->pos + ptr->collider2.offset;
		RemoveObjectFromSpace(oft);
		RegisterObjectToSpace(&ptr->collider2, oft, OFT_PLAYERBULLET);
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

	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, tex);

	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		//D3DXMatrixRotationYawPitchRoll(&mtxRot, rotAngle[currentState].y, rotAngle[currentState].x, rotAngle[currentState].z);
		GetInvRotBattleCamera(&mtxWorld);
		//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//通常描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		//加算描画
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

#ifdef _DEBUG
	ptr = &bullet[0];
	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}
		DrawBoundingCube(&ptr->collider2);
	}
#endif
}

/**************************************
プレイヤーバレットセット関数
***************************************/
void SetPlayerBullet(D3DXVECTOR3 pos, float speed)
{
	switch (GetShotLevel())
	{
	case 0:
		SetPlayerBulletLv0(pos, speed);
		break;

	case 1:
		SetPlayerBulletLv1(pos, speed);
		break;

	case 2:
		SetPlayerBulletLv2(pos, speed);
		break;

	case 3:
		SetPlayerBulletLv3(pos, speed);
		break;

	case 4:
		SetPlayerBulletLv4(pos, speed);
		break;

	default:
		SetPlayerBulletLv0(pos, speed);
		break;
	}
}

/**************************************
プレイヤーバレットセット関数Lv1
***************************************/
void SetPlayerBulletLv0(D3DXVECTOR3 pos, float speed)
{
	PLAYERBULLET *ptr = &bullet[0];

	for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->rotation = RandomRangef(0.0f, 6.12f);

		ptr->moveDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		ptr->speed = speed;

		ptr->active = true;
		return;
	}
}

/**************************************
プレイヤーバレットセット関数Lv1
***************************************/
void SetPlayerBulletLv1(D3DXVECTOR3 pos, float speed)
{
	const int shotNum = 3;

	const D3DXVECTOR3 setDir[shotNum] = {
		D3DXVECTOR3(0.1f, 0.0f, 1.0f),
		D3DXVECTOR3(-0.1f, 0.0f, 1.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	};

	for (int j = 0; j < shotNum; j++)
	{
		PLAYERBULLET *ptr = &bullet[0];
		for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
		{
			if (ptr->active)
			{
				continue;
			}

			ptr->pos = pos;
			ptr->rotation = RandomRangef(0.0f, 6.12f);

			D3DXVec3Normalize(&ptr->moveDir, &setDir[j]);

			ptr->speed = speed;

			ptr->active = true;
			break;
		}
	}
}

/**************************************
プレイヤーバレットセット関数Lv2
***************************************/
void SetPlayerBulletLv2(D3DXVECTOR3 pos, float speed)
{
	const int shotNum = 5;

	const D3DXVECTOR3 setDir[shotNum] = {
		D3DXVECTOR3(0.1f, 0.0f, 1.0f),
		D3DXVECTOR3(-0.1f, 0.0f, 1.0f),
		D3DXVECTOR3(0.0f, 0.1f, 1.0f),
		D3DXVECTOR3(0.0f, -0.1f, 1.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f)
	};

	for (int j = 0; j < shotNum; j++)
	{
		PLAYERBULLET *ptr = &bullet[0];
		for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
		{
			if (ptr->active)
			{
				continue;
			}

			ptr->pos = pos;
			ptr->rotation = RandomRangef(0.0f, 6.12f);

			D3DXVec3Normalize(&ptr->moveDir, &setDir[j]);

			ptr->speed = speed;

			ptr->active = true;
			break;
		}
	}
}

/**************************************
プレイヤーバレットセット関数Lv3
***************************************/
void SetPlayerBulletLv3(D3DXVECTOR3 pos, float speed)
{
	const int shotNum = 8;

	const D3DXVECTOR3 setDir[shotNum] = {
		D3DXVECTOR3(0.05f, 0.0f, 1.0f),
		D3DXVECTOR3(-0.05f, 0.0f, 1.0f),
		D3DXVECTOR3(0.0f, 0.05f, 1.0f),
		D3DXVECTOR3(0.0f, -0.05f, 1.0f),
		D3DXVECTOR3(0.1f, 0.1f, 1.0f),
		D3DXVECTOR3(-0.1f, 0.1f, 1.0f),
		D3DXVECTOR3(0.1f, -0.1f, 1.0f),
		D3DXVECTOR3(-0.1f, -0.1f, 1.0f),
	};

	for (int j = 0; j < shotNum; j++)
	{
		PLAYERBULLET *ptr = &bullet[0];
		for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
		{
			if (ptr->active)
			{
				continue;
			}

			ptr->pos = pos;
			ptr->rotation = RandomRangef(0.0f, 6.12f);

			D3DXVec3Normalize(&ptr->moveDir, &setDir[j]);

			ptr->speed = speed;

			ptr->active = true;
			break;
		}
	}
}

/**************************************
プレイヤーバレットセット関数Lv4
***************************************/
void SetPlayerBulletLv4(D3DXVECTOR3 pos, float speed)
{
	const int shotNum = 12;
	const float AngleRange = 0.025f;

	const D3DXVECTOR3 setDir[shotNum] = {
		D3DXVECTOR3(0.05f, 0.0f, 1.0f),
		D3DXVECTOR3(-0.05f, 0.0f, 1.0f),
		D3DXVECTOR3(0.0f, 0.05f, 1.0f),
		D3DXVECTOR3(0.0f, -0.05f, 1.0f),
		D3DXVECTOR3(0.1f, 0.1f, 1.0f),
		D3DXVECTOR3(-0.1f, 0.1f, 1.0f),
		D3DXVECTOR3(0.1f, -0.1f, 1.0f),
		D3DXVECTOR3(-0.1f, -0.1f, 1.0f),
		D3DXVECTOR3(0.2f, 0.0f, 1.0f),
		D3DXVECTOR3(-0.2f, 0.0f, 1.0f),
		D3DXVECTOR3(0.0f, 0.2f, 1.0f),
		D3DXVECTOR3(0.0f, -0.2f, 1.0f),
	};

	for (int j = 0; j < shotNum; j++)
	{
		PLAYERBULLET *ptr = &bullet[0];
		for (int i = 0; i < PLAYERBULLET_MAX; i++, ptr++)
		{
			if (ptr->active)
			{
				continue;
			}

			ptr->pos = pos;
			ptr->rotation = RandomRangef(0.0f, 6.12f);

			D3DXVECTOR3 vec = setDir[j] + D3DXVECTOR3(RandomRangef(-AngleRange, AngleRange), RandomRangef(-AngleRange, AngleRange), 0.0f);
			D3DXVec3Normalize(&ptr->moveDir, &vec);

			ptr->speed = speed;

			ptr->active = true;
			break;
		}
	}
}

/**************************************
プレイヤーバレットゲット関数
***************************************/
PLAYERBULLET *GetPlayerBulletAdr(int id)
{
	return &bullet[id];
}

/**************************************
プレイヤーバレット状態セット関数
***************************************/
void SetPlayerBulletState(int state)
{
	if (state < PlayerBulletStateMax)
		currentState = state;
}

/**************************************
頂点作成処理
***************************************/
void CreatePlayerBulletVertexBuffer(void)
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
プレイヤー爆発処理
***************************************/
void BurstPlayerBullet(PLAYERBULLET *ptr)
{
	/*RemoveObjectFromSpace(&objectForTree[ptr->id]);
	ptr->active = false;*/
	ptr->destroyRequest = true;
	SetPlayerBulletExplosion(ptr->pos);
}