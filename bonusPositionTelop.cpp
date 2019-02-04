//=====================================
//
//ボーナスポジションテロップ処理[bonusPositionTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bonusPositinoTelop.h"
#include "baseGUI.h"
#include "Easing.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define BONUSPOSITIONTELOP_TEX_NAME		"data/TEXTURE/UI/bonusPosition.png"
#define BONUSPOSITIONTELOP_SIZE_X		(300.0f)
#define BONUSPOSITIONTELOP_SIZE_Y		(300.0f)
#define BONUSPOSITIONTELOP_ANIM_MAX		(4)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static int cntFrame;
static int animIndex;
static D3DXVECTOR3 pos;
static BaseGUI *gui;
static bool active;

static const float StartScale[BONUSPOSITIONTELOP_ANIM_MAX] = {
	0.9f, 1.1f, 1.1f, 0.9f
};

static const float EndScale[BONUSPOSITIONTELOP_ANIM_MAX] = {
	1.1f, 1.1f, 0.9f, 0.9f
};

static const int AnimDuration[BONUSPOSITIONTELOP_ANIM_MAX] = {
	30, 10, 30, 10
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitBonusPositionTelop(int num)
{
	static bool initialized = false;

	if (!initialized) {
		gui = new BaseGUI((LPSTR)BONUSPOSITIONTELOP_TEX_NAME, BONUSPOSITIONTELOP_SIZE_X, BONUSPOSITIONTELOP_SIZE_Y);
		initialized = true;
	}

	cntFrame = 0;
	active = false;
	animIndex = 0;

	pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
}

/**************************************
終了処理
***************************************/
void UninitBonusPositionTelop(int num)
{
	if (num == 0)
	{
		delete gui;
	}
}

/**************************************
更新処理
***************************************/
void UpdateBonusPositionTelop(void)
{
	if (!active)
		return;

	cntFrame++;
	float t = (float)cntFrame / (float)AnimDuration[animIndex];
	float scale = EaseOutCubic(t, StartScale[animIndex], EndScale[animIndex]);
	gui->SetVertex(pos, D3DXVECTOR2(scale, scale));

	if (cntFrame == AnimDuration[animIndex])
	{
		animIndex = WrapAround(0, BONUSPOSITIONTELOP_ANIM_MAX, animIndex + 1);
		cntFrame = 0;
	}

}

/**************************************
描画処理
***************************************/
void DrawBonusPositionTelop(void)
{
	if (!active)
		return;

	gui->Draw();
}

/**************************************
状態設定処理
***************************************/
void SetStateBonusPositionTelop(bool state)
{
	active = state;
}
/**************************************
座標設定処理
***************************************/
void SetPositionBonusPositionTelop(D3DXVECTOR3 setPos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX view, proj;
	/*pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);*/

	view = GetBattleCameraView();
	proj = GetBattleCameraProjection();

	D3DXVECTOR3 screenPos;

	D3DXVec3TransformCoord(&screenPos, &setPos, &view);
	D3DXVec3TransformCoord(&screenPos, &screenPos, &proj);
	TranslateViewPort(&pos, &screenPos);
}

