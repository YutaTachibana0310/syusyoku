//=====================================
//
//HPGUI処理[hpGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hpGUI.h"
#include "GUIManager.h"
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define HPGUI_TEXTURE_NAME				_T("data/TEXTURE/UI/hpGUI.png")
#define HPGUI_TEXTURE_SIZE_X			(400.0f)
#define HPGUI_TEXTURE_SIZE_Y			(200.0f)
#define HPGUI_TEXTURE_INITPOS			(D3DXVECTOR3(SCREEN_WIDTH - HPGUI_TEXTURE_SIZE_X, -10.0f, 0.0f))

#define HPGUI_NUMTEX_NAME				_T("data/TEXTURE/UI/scoreNum.png")
#define HPGUI_NUMTEX_SIZE_X				(40.0f)
#define HPGUI_NUMTEX_SIZE_Y				(40.0f)
#define HPGUI_NUMTEX_DIVIDE_X			(5)
#define HPGUI_NUMTEX_DIVIDE_Y			(2)
#define HPGUI_NUMTEX_OFFSET				(-50.0f)
#define HPGUI_NUMTEX_INITPOS			(D3DXVECTOR3(SCREEN_WIDTH - 150.0f, 120.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;
static LPDIRECT3DTEXTURE9 numTex = NULL;
static VERTEX_2D vtxWk[NUM_VERTEX];
static HPGUI hpGUI;

static D3DXVECTOR3 backInitPos = HPGUI_TEXTURE_INITPOS;
static D3DXVECTOR2 backSize = D3DXVECTOR2(HPGUI_TEXTURE_SIZE_X, HPGUI_TEXTURE_SIZE_Y);

static D3DXVECTOR3 numInitPos = HPGUI_NUMTEX_INITPOS;
static D3DXVECTOR2 numSize = D3DXVECTOR2(HPGUI_NUMTEX_SIZE_X, HPGUI_NUMTEX_SIZE_Y);
static float numOffset = HPGUI_NUMTEX_OFFSET;

/**************************************
プロトタイプ宣言
***************************************/
HRESULT MakeVertexHPGUI(void);
void SetVertexHPGUI(void);
void SetVertexHPNum(float offset);
void SetTextureHPNum(int num);

/**************************************
初期化処理
***************************************/
void InitHpGUI(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HPGUI *ptr = &hpGUI;

	ptr->angle = atan2f(numSize.y, numSize.x);
	ptr->radius = D3DXVec2Length(&(numSize));

	MakeVertexHPGUI();

	if (!initialized)
	{
		texture = CreateTextureFromFile((LPSTR)HPGUI_TEXTURE_NAME, pDevice);
		numTex = CreateTextureFromFile((LPSTR)HPGUI_NUMTEX_NAME, pDevice);
		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitHpGUI(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
		SAFE_RELEASE(numTex);
	}
}

/**************************************
更新処理
***************************************/
void UpdateHpGUI(void)
{

}

/**************************************
描画処理
***************************************/
void DrawHpGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	//GUI背景を描画
	pDevice->SetTexture(0, texture);
	SetVertexHPGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));

	//数字を描画
	pDevice->SetTexture(0, numTex);
	int hp = 100;
	int digitMax = (hp == 0) ? 1 : (int)log10f((float)hp) + 1;
	int num = 0;
	for (int i = 0; i < digitMax; i++, hp /= 10)
	{
		num = hp % 10;

		SetVertexHPNum(i * numOffset);
		DrawGUINum(GUI_NUMSCORE, num, vtxWk);
	}
}

/**************************************
頂点作成処理
***************************************/
HRESULT MakeVertexHPGUI(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse = 
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return S_OK;
}

/**************************************
頂点設定処理
***************************************/
void SetVertexHPGUI(void)
{
	vtxWk[0].vtx = backInitPos;
	vtxWk[1].vtx = backInitPos + D3DXVECTOR3(backSize.x, 0.0f, 0.0f);
	vtxWk[2].vtx = backInitPos + D3DXVECTOR3(0.0f, backSize.y, 0.0f);
	vtxWk[3].vtx = backInitPos + D3DXVECTOR3(backSize.x, backSize.y, 0.0f);

	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/**************************************
HP数字頂点設定処理
***************************************/
void SetVertexHPNum(float offset)
{
	D3DXVECTOR3 pos = numInitPos;
	pos.x += offset;

	HPGUI *ptr = &hpGUI;
	vtxWk[0].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vtxWk[0].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vtxWk[1].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vtxWk[1].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vtxWk[2].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vtxWk[2].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
	vtxWk[3].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vtxWk[3].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
}

/**************************************
HP数字テクスチャ設定処理
***************************************/
void SetTextureHPNum(int num)
{
	int x = num % HPGUI_NUMTEX_DIVIDE_X;
	int y = num / HPGUI_NUMTEX_DIVIDE_X;
	float sizeX = 1.0f / HPGUI_NUMTEX_DIVIDE_X;
	float sizeY = 1.0f / HPGUI_NUMTEX_DIVIDE_Y;

	vtxWk[0].tex = D3DXVECTOR2(x * sizeX, y * sizeY);
	vtxWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, y * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(x * sizeX, (y + 1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, (y + 1) * sizeY);

	HPGUI *ptr = &hpGUI;

	ptr->angle = atan2f(numSize.y, numSize.x);
	ptr->radius = D3DXVec2Length(&(numSize));
}

/**************************************
デバッグウィンドウ表示
***************************************/
void DrawHPGUIDebug(void)
{
	static bool open = true;
	DebugTreeExpansion(open);
	if (DebugTreePush("HPGUI"))
	{
		DebugInputVector3(STR(backInitPos), &backInitPos);
		DebugInputVector2(STR(backSize), &backSize);

		DebugNewLine();
		DebugInputVector3(STR(numInitPos), &numInitPos);
		DebugInputVector2(STR(numSize), &numSize);
		DebugInputFloat(STR(numOffset), &numOffset);

		DebugTreePop();
	}
	HPGUI *ptr = &hpGUI;
	ptr->angle = atan2f(numSize.y, numSize.x);
	ptr->radius = D3DXVec2Length(&(numSize));

}

/**************************************
設定保存処理
***************************************/
void SaveSettinghpGUIGUI(FILE *fp)
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
void LoadSettingshpGUIGUI(FILE *fp)
{
	fread(&backInitPos, sizeof(backInitPos), 1, fp);
	fread(&backSize, sizeof(backSize), 1, fp);
	fread(&numInitPos, sizeof(numInitPos), 1, fp);
	fread(&numSize, sizeof(numSize), 1, fp);
	fread(&numOffset, sizeof(numOffset), 1, fp);
}