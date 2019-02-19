//=====================================
//
//CAUTIONテロップ処理[cautionTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "cautionTelop.h"
#include "baseGUI.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define CAUTIONTELOP_TEXTURE_NAME		"data/TEXTURE/UI/cautionTelop.png"
#define CAUTIONTELOP_TEX_SIZE_X			(SCREEN_WIDTH / 2.0f)
#define CAUTIONTELOP_TEX_SIZE_Y			(SCREEN_HEIGHT / 10.0f)
#define CAUTIONTELOP_POS_UPPER			(D3DXVECTOR3(SCREEN_CENTER_X, 50.0f, 0.0f))
#define CAUTIONTELOP_POS_LOWER			(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_HEIGHT - 50.0f, 0.0f))
#define CAUTIONTELOP_OFFSET_VALUE		(0.01f)
#define CAUTIONTELOP_LOOP_X				(4.0f)
#define CAUTIONTELOP_LOOP_Y				(1.0f)
#define CAUTIONTELOP_ANIM_MAX			(3)

/**************************************
構造体定義
***************************************/
enum CAUTIONTELOP_STATE {
	CautionTelopFadeIn,
	CautionTelopNormal,
	CautionTelopFadeOut,
	CautionTelopStateMax
};

/**************************************
グローバル変数
***************************************/
BaseGUI *telop;
static float offset;
static int currentState;
static int cntFrame;

static const int AnimDuration[CAUTIONTELOP_ANIM_MAX] = {
	60, 120, 60
};

static const float StartAlpha[CAUTIONTELOP_ANIM_MAX] = {
	0.0f, 1.0f, 1.0f
};

static const float EndAlpha[CAUTIONTELOP_ANIM_MAX] = {
	1.0f, 1.0f, 0.0f
};
/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitCautionTelop(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		telop = new BaseGUI((LPSTR)CAUTIONTELOP_TEXTURE_NAME, CAUTIONTELOP_TEX_SIZE_X, CAUTIONTELOP_TEX_SIZE_Y);
		initialized = true;
	}

	offset = 0.0f;
	currentState = CautionTelopStateMax;
}

/**************************************
終了処理
***************************************/
void UninitCautionTelop(int num)
{
	if (num == 0)
	{
		delete telop;
	}
}

/**************************************
更新処理
***************************************/
void UpdateCautionTelop(void)
{
	if (currentState >= CautionTelopStateMax)
		return;

	offset += CAUTIONTELOP_OFFSET_VALUE;
	if (offset > 1.0f)
		offset -= 1.0f;

	cntFrame++;
	float t = (float)cntFrame / (float)AnimDuration[currentState];
	float alpha = EaseLinear(t, StartAlpha[currentState], EndAlpha[currentState]);
	telop->SetAlpha(alpha);

	if (cntFrame == AnimDuration[currentState])
	{
		currentState++;
		cntFrame = 0;
	}
}

/**************************************
描画処理
***************************************/
void DrawCautionTelop(void)
{
	if (currentState >= CautionTelopStateMax)
		return;

	//上部テロップ描画
	telop->SetVertex(CAUTIONTELOP_POS_UPPER);
	telop->SetLoop(CAUTIONTELOP_LOOP_X, CAUTIONTELOP_LOOP_Y, offset, 0.0f);
	telop->Draw();

	//下部テロップ描画
	telop->SetVertex(CAUTIONTELOP_POS_LOWER);
	telop->SetLoop(CAUTIONTELOP_LOOP_X, CAUTIONTELOP_LOOP_Y, -offset, 0.0f);
	telop->Draw();
}

/**************************************
テロップセット処理
***************************************/
void SetCautionTelop(void)
{
	cntFrame = 0;
	currentState = CautionTelopFadeIn;
	offset = 0.0f;
}