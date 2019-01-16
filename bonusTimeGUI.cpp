//=====================================
//
//ボーナスタイムGUI処理[bonusTimeGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bonusTimeGUI.h"
#include "GUIManager.h"

/**************************************
マクロ定義
***************************************/
#define BONUSTIMEGUI_TEX_NAME			"data/TEXTURE/UI/bonusTimeGUI.png"
#define BONUSTIMEGUI_TEX_SIZE_X			(150.0f)
#define BONUSTIMEGUI_TEX_SIZE_Y			(80.0f)
#define BONUSTIMEGUI_DISP_POS			(D3DXVECTOR3(SCREEN_CENTER_X, BONUSTIMEGUI_TEX_SIZE_Y, 0.0f))

#define BONUSTIMEGUI_FLOAT_DIGITMAX		(2)
#define BONUSTIMEGUI_NUM_SIZE_X			(40.0f)
#define BONUSTIMEGUI_NUM_SIZE_Y			(40.0f)
#define BONUSTIMEGUI_NUM_OFFSET			(-55.0f)
#define BONUSTIMEGUI_INT_BASEPOS		(D3DXVECTOR3(SCREEN_CENTER_X - 45.0f, BONUSTIMEGUI_TEX_SIZE_Y + 30.0f, 0.0f))
#define BONUSTIMEGUI_FLOAT_BASEPOS		(D3DXVECTOR3(SCREEN_CENTER_X + 95.0f , BONUSTIMEGUI_TEX_SIZE_Y + 30.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static VERTEX_2D vtxWk[NUM_VERTEX];
static int currentCount, measureCount;
static float angleGUI, radiusGUI;
static float angleNum, radiusNum;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexBonusTimeGUI(void);
void SetVertexBonusTimeGUI(void);
void SetTextureBonusTimeGUI(void);
void SetVertexBonusTimeNum(float offset, D3DXVECTOR3 basePos);

/**************************************
初期化処理
***************************************/
void InitBonusTimeGUI(int num)
{
	if (num == 0)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		texture = CreateTextureFromFile((LPSTR)BONUSTIMEGUI_TEX_NAME, pDevice);
		MakeVertexBonusTimeGUI();
	}

	measureCount = currentCount = 1800;
}

/**************************************
終了処理
***************************************/
void UninitBonusTimeGUI(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateBonusTimeGUI(void)
{
	if (currentCount <= 0)
		return;

	currentCount--;
}

/**************************************
描画処理
***************************************/
void DrawBonusTimeGUI(void)
{
	if (currentCount <= 0)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetFVF(FVF_VERTEX_2D);

	//GUIの描画
	pDevice->SetTexture(0, texture);
	SetVertexBonusTimeGUI();
	SetTextureBonusTimeGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));

	//残り時間を計算
	float time = currentCount / 60.0f;
	int numInt = (int)time;
	int numFloat = (int)((time - numInt) * 100);

	//整数部分を描画
	{
		int digitMax = (numInt == 0) ? 1 : (int)log10f((float)numInt) + 1;
		for (int i = 0; i < digitMax; i++, numInt /= 10)
		{
			int num = numInt % 10;
			SetVertexBonusTimeNum(i * BONUSTIMEGUI_NUM_OFFSET, BONUSTIMEGUI_INT_BASEPOS);
			DrawGUINum(GUI_NUMBONUSTIME, num, vtxWk);
		}
	}

	//小数部分を描画
	{
		int digitMax = (numFloat == 0) ? 1 : (int)log10f((float)numFloat) + 1;
		for (int i = 0; i < BONUSTIMEGUI_FLOAT_DIGITMAX; i++, numFloat /= 10)
		{
			int num = (i <digitMax) ? numFloat % 10 : 0;
			SetVertexBonusTimeNum(i * BONUSTIMEGUI_NUM_OFFSET, BONUSTIMEGUI_FLOAT_BASEPOS);
			DrawGUINum(GUI_NUMBONUSTIME, num, vtxWk);
		}
	}


}

/**************************************
頂点作成処理
***************************************/
void MakeVertexBonusTimeGUI(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	radiusGUI = D3DXVec2Length(&D3DXVECTOR2(BONUSTIMEGUI_TEX_SIZE_X, BONUSTIMEGUI_TEX_SIZE_Y));
	angleGUI = atan2f(BONUSTIMEGUI_TEX_SIZE_Y, BONUSTIMEGUI_TEX_SIZE_X);

	radiusNum = D3DXVec2Length(&D3DXVECTOR2(BONUSTIMEGUI_NUM_SIZE_X, BONUSTIMEGUI_NUM_SIZE_Y));
	angleNum = atan2f(BONUSTIMEGUI_NUM_SIZE_Y, BONUSTIMEGUI_NUM_SIZE_X);
}

/**************************************
GUI用頂点設定処理
***************************************/
void SetVertexBonusTimeGUI(void)
{
	vtxWk[0].vtx.x = BONUSTIMEGUI_DISP_POS.x - cosf(angleGUI) * radiusGUI;
	vtxWk[0].vtx.y = BONUSTIMEGUI_DISP_POS.y - sinf(angleGUI) * radiusGUI;
	vtxWk[1].vtx.x = BONUSTIMEGUI_DISP_POS.x + cosf(angleGUI) * radiusGUI;
	vtxWk[1].vtx.y = BONUSTIMEGUI_DISP_POS.y - sinf(angleGUI) * radiusGUI;
	vtxWk[2].vtx.x = BONUSTIMEGUI_DISP_POS.x - cosf(angleGUI) * radiusGUI;
	vtxWk[2].vtx.y = BONUSTIMEGUI_DISP_POS.y + sinf(angleGUI) * radiusGUI;
	vtxWk[3].vtx.x = BONUSTIMEGUI_DISP_POS.x + cosf(angleGUI) * radiusGUI;
	vtxWk[3].vtx.y = BONUSTIMEGUI_DISP_POS.y + sinf(angleGUI) * radiusGUI;
}

/**************************************
GUI用UV座標設定処理
***************************************/
void SetTextureBonusTimeGUI(void)
{
	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/**************************************
数字用頂点座標設定処理
***************************************/
void SetVertexBonusTimeNum(float offset, D3DXVECTOR3 basePos)
{
	vtxWk[0].vtx.x = basePos.x - cosf(angleNum) * radiusNum + offset;
	vtxWk[0].vtx.y = basePos.y - sinf(angleNum) * radiusNum;
	vtxWk[1].vtx.x = basePos.x + cosf(angleNum) * radiusNum + offset;
	vtxWk[1].vtx.y = basePos.y - sinf(angleNum) * radiusNum;
	vtxWk[2].vtx.x = basePos.x - cosf(angleNum) * radiusNum + offset;
	vtxWk[2].vtx.y = basePos.y + sinf(angleNum) * radiusNum;
	vtxWk[3].vtx.x = basePos.x + cosf(angleNum) * radiusNum + offset;
	vtxWk[3].vtx.y = basePos.y + sinf(angleNum) * radiusNum;
}

