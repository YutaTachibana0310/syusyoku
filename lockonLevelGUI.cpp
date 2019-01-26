//=====================================
//
//ロックオンレベルGUI処理[lockonLevelGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "lockonLevelGUI.h"
#include "GUIManager.h"
#include "dataContainer.h"
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define LOCKONLEVELGUI_TEX_NAME					"data/TEXTURE/UI/lockonlevel.png"
#define LOCKONLEVELGUI_TEX_SIZE_X				(400.0f)
#define LOCKONLEVELGUI_TEX_SIZE_Y				(200.0f)
#define LOCKONLEVELGUI_TEX_INITPOS				(D3DXVECTOR3(SCREEN_WIDTH - LOCKONLEVELGUI_TEX_SIZE_X, 220.0f, 0.0f))

#define LOCKONLEVELGUI_NUMTEX_SIZE_X			(80.0f)
#define LOCKONLEVELGUI_NUMTEX_SIZE_Y			(80.0f)
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

static D3DXVECTOR3 backInitPos = LOCKONLEVELGUI_TEX_INITPOS;
static D3DXVECTOR2 backSize = D3DXVECTOR2(LOCKONLEVELGUI_TEX_SIZE_X, LOCKONLEVELGUI_TEX_SIZE_Y);

static D3DXVECTOR3 numInitPos = LOCKONLEVELGUI_NUMTEX_INITPOS;
static D3DXVECTOR2 numSize = D3DXVECTOR2(LOCKONLEVELGUI_NUMTEX_SIZE_X, LOCKONLEVELGUI_NUMTEX_SIZE_Y);
static float numOffset = LOCKONLEVELGUI_NUMTEX_OFFSET;

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
頂点設定処理
***************************************/
void SetVertexLockonLevelGUI(void)
{
	vtxWk[0].vtx = backInitPos;
	vtxWk[1].vtx = backInitPos + D3DXVECTOR3(backSize.x, 0.0f, 0.0f);
	vtxWk[2].vtx = backInitPos + D3DXVECTOR3(0.0f, backSize.y, 0.0f);
	vtxWk[3].vtx = backInitPos + D3DXVECTOR3(backSize.x, backSize.y, 0.0f);
}

/**************************************
テクスチャ座標設定処理
***************************************/
void SetTextureLockonLevelGUI(void)
{
	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/**************************************
頂点設定処理
***************************************/
void SetVertexLockonLevelNUM(void)
{
	vtxWk[0].vtx = numInitPos;
	vtxWk[1].vtx = numInitPos + D3DXVECTOR3(numSize.x, 0.0f, 0.0f);
	vtxWk[2].vtx = numInitPos + D3DXVECTOR3(0.0f, numSize.y, 0.0f);
	vtxWk[3].vtx = numInitPos + D3DXVECTOR3(numSize.x, numSize.y, 0.0f);
}

/**************************************
デバッグウィンドウ表示
***************************************/
void DrawLockOnlevelGUIDebug(void)
{
	static bool open = true;
	DebugTreeExpansion(open);
	if (DebugTreePush("LockonLevelGUI"))
	{
		DebugInputVector3(STR(backInitPos), &backInitPos);
		DebugInputVector2(STR(backSize), &backSize);

		DebugNewLine();
		DebugInputVector3(STR(numInitPos), &numInitPos);
		DebugInputVector2(STR(numSize), &numSize);
		DebugInputFloat(STR(numOffset), &numOffset);

		DebugTreePop();
	}

}

/**************************************
設定保存処理
***************************************/
void SaveSettingLockonLevelGUI(FILE *fp)
{
	fwrite(&backInitPos, sizeof(backInitPos), 1, fp);
	fwrite(&backSize, sizeof(backSize), 1, fp);
	fwrite(&numInitPos, sizeof(numInitPos), 1, fp);
	fwrite(&numSize, sizeof(numSize), 1, fp);
	fwrite(&numOffset, sizeof(numOffset), 1, fp);
}

/**************************************
設定読み込み処理
***************************************/
void LoadSettingsLockonLevelGUI(FILE *fp)
{
	fread(&backInitPos, sizeof(backInitPos), 1, fp);
	fread(&backSize, sizeof(backSize), 1, fp);
	fread(&numInitPos, sizeof(numInitPos), 1, fp);
	fread(&numSize, sizeof(numSize), 1, fp);
	fread(&numOffset, sizeof(numOffset), 1, fp);
}