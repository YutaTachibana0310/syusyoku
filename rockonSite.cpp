//=====================================
//
//テンプレート処理[rockonSite.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "rockonSite.h"
#include "playerModel.h"
#include "battleCamera.h"
#include "debugproc.h"

/**************************************
マクロ定義
***************************************/
#define ROCKONSITE_SIZE		(10)
#define ROCKONSITE_TEXTURE	"data/TEXTURE/rockonSite.png"
#define ROCKONSITE_MAX		(128)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static ROCKONSITE rockon[ROCKONSITE_MAX * PLAYERMODEL_MAX];
static VERTEX_2D vtxWk[NUM_VERTEX];

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexRockonSite(void);
void SetVertexRockonSite(ROCKONSITE *ptr);

/**************************************
初期化処理
***************************************/
void InitRockonSite(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		texture = CreateTextureFromFile((LPSTR)ROCKONSITE_TEXTURE, pDevice);

		MakeVertexRockonSite();
	}

	ROCKONSITE *ptr = &rockon[0];
	for (int i = 0; i < ROCKONSITE_MAX; i++, ptr++)
	{
		ptr->active = false;
		ptr->pos = D3DXVECTOR3(-9999.9f, -9999.9f, -9999.9f);
	}
}

/**************************************
終了処理
***************************************/
void UninitRockonSite(int num)
{
	ROCKONSITE *ptr = &rockon[0];
	for (int i = 0; i < ROCKONSITE_MAX; i++, ptr++)
	{
		ptr->active = false;
	}

	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateRockonSite(void)
{
	ROCKONSITE *ptr = &rockon[0];

	int cnt = 0;

	for (int i = 0; i < ROCKONSITE_MAX * PLAYERMODEL_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//サイトの座標をターゲットと同じ位置に設定
		ptr->pos = *ptr->target;

		//ビュー変換、プロジェクション変換を行う
		D3DXVec3TransformCoord(&ptr->pos, &ptr->pos, &GetBattleCameraView());
		D3DXVec3TransformCoord(&ptr->pos, &ptr->pos, &GetBattleCameraProjection());

		//ビューポート変換でスクリーン座標に変換する
		ptr->pos.x = ptr->pos.x * (SCREEN_WIDTH / 2.0f) + SCREEN_WIDTH / 2.0f;
		ptr->pos.y = ptr->pos.y * (-SCREEN_HEIGHT / 2.0f) + SCREEN_HEIGHT / 2.0f;

		cnt++;
		if (ptr->target->z < 0.0f)
		{
			ptr->target = NULL;
			ptr->active = false;
		}
	}

	PrintDebugProc("rockon:%d\n", cnt);
}

/**************************************
描画処理
***************************************/
void DrawRockonSite(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, texture);

	ROCKONSITE *ptr = &rockon[0];
	for (int i = 0; i < ROCKONSITE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		SetVertexRockonSite(ptr);

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
	}
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexRockonSite(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/**************************************
頂点設定処理
***************************************/
void SetVertexRockonSite(ROCKONSITE *ptr)
{
	vtxWk[0].vtx.x = ptr->pos.x - ROCKONSITE_SIZE;
	vtxWk[0].vtx.y = ptr->pos.y - ROCKONSITE_SIZE;
	vtxWk[1].vtx.x = ptr->pos.x + ROCKONSITE_SIZE;
	vtxWk[1].vtx.y = ptr->pos.y - ROCKONSITE_SIZE;
	vtxWk[2].vtx.x = ptr->pos.x - ROCKONSITE_SIZE;
	vtxWk[2].vtx.y = ptr->pos.y + ROCKONSITE_SIZE;
	vtxWk[3].vtx.x = ptr->pos.x + ROCKONSITE_SIZE;
	vtxWk[3].vtx.y = ptr->pos.y + ROCKONSITE_SIZE;
}

/**************************************
セット処理
***************************************/
void SetRockonSite(int id, D3DXVECTOR3 *target)
{
	ROCKONSITE *ptr = &rockon[id * ROCKONSITE_MAX];
	int end = (id + 1) * ROCKONSITE_MAX;

	for (int i = 0; i < end; i++, ptr++)
	{
		if (ptr->target == target)
		{
			return;
		}

		if (ptr->target == NULL)
		{
			ptr->active = true;
			ptr->target = target;
			return;
		}



	}
}