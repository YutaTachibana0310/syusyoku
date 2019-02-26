//=====================================
//
//スパイクノイズ処理[spikeNoise.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "spikeNoise.h"
#include "debugWindow.h"
#include "postEffectManager.h"
#include "Easing.h"
#include "soundEffectManager.h"

/**************************************
マクロ定義
***************************************/
#define SPIKENOISE_EFFECT_NAME		"data/EFFECT/spikeNoise.fx"
#define SPIKENOISE_EFFECT_DURATION	(30)
#define SPIKENOISE_EFFECTLENGTH_MAX	(0.02f)

/**************************************
構造体定義
***************************************/
static LPD3DXEFFECT effect;
static D3DXHANDLE tech, length;
static bool active;
static int cntFrame;

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitSpikeNoise(int num)
{
	static bool initialized = false;
	active = false;
	cntFrame = 0;

	if (initialized)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, SPIKENOISE_EFFECT_NAME, 0, 0, 0, 0, &effect, NULL);

	tech = effect->GetTechniqueByName("tech");
	length = effect->GetParameterByName(NULL, "length");

	effect->SetTechnique(tech);

	initialized = true;
}

/**************************************
終了処理
***************************************/
void UninitSpikeNoise(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(effect);
	}
}

#include "input.h"
/**************************************
更新処理
***************************************/
void UpdateSpikeNoise(void)
{
	if (GetKeyboardTrigger(DIK_A))
	{
		SetSpikeNoise();
	}


	if (!active)
		return;

	cntFrame++;

	float t = (float)cntFrame / (float)SPIKENOISE_EFFECT_DURATION;
	float effectLength = EaseOutCubic(t, 1.0f, 0.0f);
	effect->SetFloat(length, effectLength);

	if (cntFrame == SPIKENOISE_EFFECT_DURATION)
	{
		active = false;
		SetPostEffectUse(EFFECT_SPIKENOISE, false);
	}
}

/**************************************
描画処理
***************************************/
void DrawSpikeNoise(void)
{
	if (!active)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	effect->Begin(NULL, 0);
	effect->BeginPass(0);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	effect->EndPass();
	effect->End();
}

/**************************************
セット処理
***************************************/
void SetSpikeNoise(void)
{
	cntFrame = 0;
	active = true;
	SetPostEffectUse(EFFECT_SPIKENOISE, true);
	//PlaySE(DefineSE::NOISE);
}
