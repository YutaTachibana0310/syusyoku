//=====================================
//
//ロックオンGUI処理[lockonGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "lockonGUI.h"
#include "playerModel.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define LOCKONGUI_TEXTTEX_NAME			"data/TEXTURE/UI/lockonText.png"
#define LOCKONGUI_TEXTTEX_DIVIDE_X		(1)
#define LOCKONGUI_TEXTTEX_DIVIDE_Y		(2)
#define LOCKONGUI_TEXTTEX_PATTERN		(2)
#define LOCKONGUI_TEXTTEX_SIZE_X		(128)
#define LOCKONGUI_TEXTTEX_SIZE_Y		(64)

/**************************************
構造体定義
***************************************/
enum LOCKONGUI_STATE
{
	LOCKONGUI_LOCKABLE,
	LOCKONGUI_RECGARGE,
	LOCKONGUI_STATEMAX,
};
/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];
static LPDIRECT3DTEXTURE9 texture;
static LOCKONGUI lockonGUI[PLAYERMODEL_MAX];

/**************************************
プロトタイプ宣言
***************************************/
void CreateVertexLockonGUI(void);
void SetLockonGUITextVertex(int id);

/**************************************
初期化処理
***************************************/
void InitLockonGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LOCKONGUI *ptr = &lockonGUI[0];

	if (num == 0)
	{
		CreateVertexLockonGUI();

		texture = CreateTextureFromFile((LPSTR)LOCKONGUI_TEXTTEX_NAME, pDevice);
	}

	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
	{
		ptr->active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitLockonGUI(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateLockonGUI(void)
{

}

/**************************************
描画処理
***************************************/
void DrawLockonGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LOCKONGUI *ptr = &lockonGUI[0];
	D3DXMATRIX mtxTranslate, mtxWorld;

	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//テキストを描画
		pDevice->SetTexture(0, texture);
		SetLockonGUITextVertex(i);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
		
		
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

/**************************************
頂点作成処理
***************************************/
void CreateVertexLockonGUI(void)
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
座標設定処理
***************************************/
void SetLockonGUIPos(int id, D3DXVECTOR3 pos)
{
	LOCKONGUI *ptr = &lockonGUI[id];

	ptr->pos = pos;

	//設定した座標をスクリーン座標へ変換
	D3DXVec3TransformCoord(&ptr->pos, &ptr->pos, &GetBattleCameraView());
	D3DXVec3TransformCoord(&ptr->pos, &ptr->pos, &GetBattleCameraProjection());
	TranslateViewPort(&ptr->pos, &ptr->pos);
	ptr->pos.z = 0.0f;
}

/**************************************
テキストテクスチャ設定処理
***************************************/
void SetLockonGUITextVertex(int id)
{
	PLAYERMODEL *player = GetPlayerAdr(id);
	LOCKONGUI *ptr = &lockonGUI[id];

	int pattern = (player->atkInterbal >= PLAYER_HOMINGATK_INTERBAL) ? 0 : 1;
	float sizeY = 1.0f / LOCKONGUI_TEXTTEX_DIVIDE_Y;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, sizeY * pattern);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, sizeY * pattern);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, sizeY * (pattern + 1));
	vtxWk[3].tex = D3DXVECTOR2(1.0f, sizeY * (pattern + 1));

	vtxWk[0].vtx = ptr->pos + D3DXVECTOR3(-LOCKONGUI_TEXTTEX_SIZE_X, -LOCKONGUI_TEXTTEX_SIZE_Y, 0.0f);
	vtxWk[1].vtx = ptr->pos + D3DXVECTOR3(LOCKONGUI_TEXTTEX_SIZE_X, -LOCKONGUI_TEXTTEX_SIZE_Y, 0.0f);
	vtxWk[2].vtx = ptr->pos + D3DXVECTOR3(-LOCKONGUI_TEXTTEX_SIZE_X, LOCKONGUI_TEXTTEX_SIZE_Y, 0.0f);
	vtxWk[3].vtx = ptr->pos + D3DXVECTOR3(LOCKONGUI_TEXTTEX_SIZE_X, LOCKONGUI_TEXTTEX_SIZE_Y, 0.0f);
}

/**************************************
ロックオンGUIアドレス取得処理
***************************************/
LOCKONGUI *GetLockonGUIAdr(int id)
{
	return &lockonGUI[id];
}