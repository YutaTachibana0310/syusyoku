//=====================================
//
//ロックオン数GUI処理[lockonNumGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "lockonNumGUI.h"
#include "GUIManager.h"
#include "dataContainer.h"
#include "playerModel.h"
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define LOCKONNUMGUI_TEX_NAME				"data/TEXTURE/UI/lockonGUI.png"
#define LOCKONNUMGUI_TEX_SIZE_X				(400.0f)
#define LOCKONNUMGUI_TEX_SIZE_Y				(200.0f)
#define LOCKONNUMGUI_TEX_INITPOS			(D3DXVECTOR3(SCREEN_WIDTH - LOCKONNUMGUI_TEX_SIZE_X, 430.0f, 0.0f))

#define LOCKONNUMGUI_NUMTEX_SIZE_X			(80.0f)
#define LOCKONNUMGUI_NUMTEX_SIZE_Y			(80.0f)
#define LOCKONNUMGUI_NUMTEX_OFFSET			(-50.0f)
#define LOCKONNUMGUI_NUMTEX_INITPOS			(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 525.0f, 0.0f))

#define LOCKONUMGUI_MAXTEX_INITPOS			(D3DXVECTOR3(SCREEN_WIDTH - 90.0f, 525.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static VERTEX_2D vtxWk[NUM_VERTEX];

static D3DXVECTOR3 backInitPos = LOCKONNUMGUI_TEX_INITPOS;
static D3DXVECTOR2 backSize = D3DXVECTOR2(LOCKONNUMGUI_TEX_SIZE_X, LOCKONNUMGUI_TEX_SIZE_Y);

static D3DXVECTOR3 numInitPos = LOCKONNUMGUI_NUMTEX_INITPOS;
static D3DXVECTOR3 maxInitPos = LOCKONUMGUI_MAXTEX_INITPOS;
static D3DXVECTOR2 numSize = D3DXVECTOR2(LOCKONNUMGUI_NUMTEX_SIZE_X, LOCKONNUMGUI_NUMTEX_SIZE_Y);
static float numOffset = LOCKONNUMGUI_NUMTEX_OFFSET;
/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexLockonNumGUI(void);
void SetVertexLockonNumGUI(void);
void SetTextureLockonNumGUI(void);
void SetVertexLockonNumNUM(float offsetX, bool isMaxNum);

/**************************************
初期化処理
***************************************/
void InitLockonNumGUI(int num)
{
	static bool initialized = false;

	if (initialized)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	texture = CreateTextureFromFile((LPSTR)LOCKONNUMGUI_TEX_NAME, pDevice);
	MakeVertexLockonNumGUI();
}

/**************************************
終了処理
***************************************/
void UninitLockonNumGUI(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateLockonNumGUI(void)
{

}

/**************************************
描画処理
***************************************/
void DrawLockonNumGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	//GUI背景を描画
	pDevice->SetTexture(0, texture);
	SetVertexLockonNumGUI();
	SetTextureLockonNumGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));

	//ロックオン数を描画
	{
		int lockNum = GetPlayerAdr(0)->lockonNum;
		int digitMax = (lockNum == 0) ? 1 : (int)log10f((float)lockNum) + 1;
		for (int i = 0; i < digitMax; i++, lockNum /= 10)
		{
			int num = lockNum % 10;
			SetVertexLockonNumNUM(i * LOCKONNUMGUI_NUMTEX_OFFSET, false);
			DrawGUINum(GUI_NUMSCORE, num, vtxWk);
		}
	}

	//最大ロックオン数を描画
	{
		int lockNum = GetLockonMax();
		int digitMax = (lockNum == 0) ? 1 : (int)log10f((float)lockNum) + 1;
		for (int i = 0; i < digitMax; i++, lockNum /= 10)
		{
			int num = lockNum % 10;
			SetVertexLockonNumNUM(i * LOCKONNUMGUI_NUMTEX_OFFSET, true);
			DrawGUINum(GUI_NUMSCORE, num, vtxWk);
		}
	}
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexLockonNumGUI(void)
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
void SetVertexLockonNumGUI(void)
{
	vtxWk[0].vtx = backInitPos;
	vtxWk[1].vtx = backInitPos + D3DXVECTOR3(backSize.x, 0.0f, 0.0f);
	vtxWk[2].vtx = backInitPos + D3DXVECTOR3(0.0f, backSize.y, 0.0f);
	vtxWk[3].vtx = backInitPos + D3DXVECTOR3(backSize.x, backSize.y, 0.0f);
}

/**************************************
テクスチャ座標設定処理
***************************************/
void SetTextureLockonNumGUI(void)
{
	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/**************************************
頂点設定処理
***************************************/
void SetVertexLockonNumNUM(float offsetX, bool isMaxNum)
{
	D3DXVECTOR3 basePos = (isMaxNum) ? maxInitPos : numInitPos;

	vtxWk[0].vtx = basePos;
	vtxWk[1].vtx = basePos + D3DXVECTOR3(numSize.x, 0.0f, 0.0f);
	vtxWk[2].vtx = basePos + D3DXVECTOR3(0.0f, numSize.y, 0.0f);
	vtxWk[3].vtx = basePos + D3DXVECTOR3(numSize.x, numSize.y, 0.0f);

	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vtxWk[i].vtx.x += offsetX;
	}
}

/**************************************
デバッグウィンドウ表示
***************************************/
void DrawLockNumlevelGUIDebug(void)
{
	static bool open = true;
	DebugTreeExpansion(open);
	if (DebugTreePush("LockonNumGUI"))
	{
		DebugInputVector3(STR(backInitPos), &backInitPos);
		DebugInputVector2(STR(backSize), &backSize);

		DebugNewLine();
		DebugInputVector3(STR(numInitPos), &numInitPos);
		DebugInputVector3(STR(maxInitPos), &maxInitPos);
		DebugInputVector2(STR(numSize), &numSize);
		DebugInputFloat(STR(numOffset), &numOffset);

		DebugTreePop();
	}

}

/**************************************
設定保存処理
***************************************/
void SaveSettingLockonNumGUI(FILE *fp)
{
	fwrite(&backInitPos, sizeof(backInitPos), 1, fp);
	fwrite(&backSize, sizeof(backSize), 1, fp);
	fwrite(&numInitPos, sizeof(numInitPos), 1, fp);
	fwrite(&maxInitPos, sizeof(maxInitPos), 1, fp);
	fwrite(&numSize, sizeof(numSize), 1, fp);
	fwrite(&numOffset, sizeof(numOffset), 1, fp);
}

/**************************************
設定読み込み処理
***************************************/
void LoadSettingsLockonNumGUI(FILE *fp)
{
	fread(&backInitPos, sizeof(backInitPos), 1, fp);
	fread(&backSize, sizeof(backSize), 1, fp);
	fread(&numInitPos, sizeof(numInitPos), 1, fp);
	fread(&numSize, sizeof(numSize), 1, fp);
	fread(&numOffset, sizeof(numOffset), 1, fp);
}