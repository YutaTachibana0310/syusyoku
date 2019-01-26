//=====================================
//
//ロックオンレベルGUI処理[lockonLevelGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "lockonLevelGUI.h"
#include "GUIManager.h"
#include "dataContainer.h"

/**************************************
マクロ定義
***************************************/
#define LOCKONLEVELGUI_TEX_NAME					"data/TEXTURE/UI/lockonlevel.png"
#define LOCKONLEVELGUI_TEX_SIZE_X				(400.0f)
#define LOCKONLEVELGUI_TEX_SIZE_Y				(200.0f)
#define LOCKONLEVELGUI_TEX_INITPOS				(D3DXVECTOR3(SCREEN_WIDTH - LOCKONLEVELGUI_TEX_SIZE_X, 220.0f, 0.0f))

#define LOCKONLEVELGUI_NUMTEX_SIZE_X			(80.0f)
#define LOCKONLEVELGUI_NUMTEX_SIZE_Y			(80.0f)
#define LOCKONLEVELGUI_NUMTEX_DIVIDE_X			(5)
#define LOCKONLEVELGUI_NUMTEX_DIVIDE_Y			(2)
#define LOCKONLEVELGUI_NUMTEX_OFFSET			(-50.0f)
#define LOCKONLEVELGUI_NUMTEX_INITPOS			(D3DXVECTOR3(SCREEN_WIDTH - 120.0f, 305.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static VERTEX_2D vtxWk[NUM_VERTEX];

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexLockonLevelGUI(void);
void SetVertexLockonLevelGUI(void);
void SetTextureLockonLevelGUI(void);
void SetVertexLockonLevelNUM(void);

/**************************************
初期化処理
***************************************/
void InitLockonLevelGUI(int num)
{
	static bool initialized = false;

	if (initialized)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	texture = CreateTextureFromFile((LPSTR)LOCKONLEVELGUI_TEX_NAME, pDevice);
	MakeVertexLockonLevelGUI();
}

/**************************************
終了処理
***************************************/
void UninitLockonLevelGUI(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateLockonLevelGUI(void)
{

}

/**************************************
描画処理
***************************************/
void DrawLockonLevelGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	//GUI背景を描画
	pDevice->SetTexture(0, texture);
	SetVertexLockonLevelGUI();
	SetTextureLockonLevelGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));

	//数字を描画
	SetVertexLockonLevelNUM();
	DrawGUINum(GUI_NUMSCORE, GetLockonLevel() + 1, vtxWk);
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexLockonLevelGUI(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

/**************************************
描画処理
***************************************/
void SetVertexLockonLevelGUI(void)
{
	vtxWk[0].vtx = LOCKONLEVELGUI_TEX_INITPOS;
	vtxWk[1].vtx = LOCKONLEVELGUI_TEX_INITPOS + D3DXVECTOR3(LOCKONLEVELGUI_TEX_SIZE_X, 0.0f, 0.0f);
	vtxWk[2].vtx = LOCKONLEVELGUI_TEX_INITPOS + D3DXVECTOR3(0.0f, LOCKONLEVELGUI_TEX_SIZE_Y, 0.0f);
	vtxWk[3].vtx = LOCKONLEVELGUI_TEX_INITPOS + D3DXVECTOR3(LOCKONLEVELGUI_TEX_SIZE_X,LOCKONLEVELGUI_TEX_SIZE_Y, 0.0f);
}

/**************************************
描画処理
***************************************/
void SetTextureLockonLevelGUI(void)
{
	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/**************************************
描画処理
***************************************/
void SetVertexLockonLevelNUM(void)
{
	vtxWk[0].vtx = LOCKONLEVELGUI_NUMTEX_INITPOS;
	vtxWk[1].vtx = LOCKONLEVELGUI_NUMTEX_INITPOS + D3DXVECTOR3(LOCKONLEVELGUI_NUMTEX_SIZE_X, 0.0f, 0.0f);
	vtxWk[2].vtx = LOCKONLEVELGUI_NUMTEX_INITPOS + D3DXVECTOR3(0.0f, LOCKONLEVELGUI_NUMTEX_SIZE_Y, 0.0f);
	vtxWk[3].vtx = LOCKONLEVELGUI_NUMTEX_INITPOS + D3DXVECTOR3(LOCKONLEVELGUI_NUMTEX_SIZE_X, LOCKONLEVELGUI_NUMTEX_SIZE_Y, 0.0f);
}