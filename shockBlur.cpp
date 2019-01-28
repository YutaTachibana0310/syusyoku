//=====================================
//
//衝撃ブラー処理[shockBlur.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "shockBlur.h"
#include <assert.h>
#include "debugWindow.h"
#include "postEffectManager.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define SHOCKBLUR_EFFECT_NAME	"data/EFFECT/shockBlur.fx"
#define SHOCLBLUR_POWER			(15.0f)
#define SHOCKBLUR_ANIM_MAX		(3)

/**************************************
構造体定義
***************************************/


/**************************************
グローバル変数
***************************************/
static LPD3DXEFFECT effect = NULL;							//シェーダ
static D3DXHANDLE tech, centerTexel, tU, tV, blurPower;		//シェーダ内グローバル変数
static bool active = false;
static int cntFrame;
static int animIndex;

static const float AnimDuration[SHOCKBLUR_ANIM_MAX] = {
	5, 20, 5
};

static const float AnimStartPower[SHOCKBLUR_ANIM_MAX] = {
	0.0f, SHOCLBLUR_POWER, SHOCLBLUR_POWER
};

static const float AnimEndPower[SHOCKBLUR_ANIM_MAX] = {
	0.0f, SHOCLBLUR_POWER, 0.0f
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitShcokBlur(int num)
{
	static bool initialized = false;

	active = true;
	cntFrame = 0;
	animIndex = 0;

	if (initialized)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, SHOCKBLUR_EFFECT_NAME, 0, 0, 0, 0, &effect, 0);

	if (effect == NULL)
	{
		assert(effect);
		return;
	}

	tech = effect->GetTechniqueByName("tech");
	centerTexel = effect->GetParameterByName(NULL, STR(centerTexel));
	tU = effect->GetParameterByName(NULL, STR(tU));
	tV = effect->GetParameterByName(NULL, STR(tV));
	blurPower = effect->GetParameterByName(NULL, STR(blurPower));

	float texelU = 1.0f / SCREEN_WIDTH;
	float texelV = 1.0f / SCREEN_HEIGHT;

	effect->SetFloat(tU, texelU);
	effect->SetFloat(tV, texelV);
	effect->SetFloat(blurPower, SHOCLBLUR_POWER);

	effect->SetTechnique(tech);

	initialized = true;
}

/**************************************
終了処理
***************************************/
void UninitShcokBlur(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(effect);
	}
}

/**************************************
更新処理
***************************************/
void UpdateShcokBlur(void)
{
	if (!active)
		return;

	cntFrame++;

	//ブラーの強さをアニメーション
	float t = (float)cntFrame / (float)AnimDuration[animIndex];
	float power = EaseLinear(t, AnimStartPower[animIndex], AnimEndPower[animIndex]);
	effect->SetFloat(blurPower, power);

	//アニメーション遷移判定
	if (cntFrame >= AnimDuration[animIndex])
	{
		animIndex++;
		cntFrame = 0;

		//終了判定
		if (animIndex == SHOCKBLUR_ANIM_MAX)
		{
			active = false;
			SetPostEffectUse(EFFECT_SHOCKBLUR, false);
		}
	}
}

/**************************************
描画処理
***************************************/
void DrawShcokBlur(void)
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
void SetShockBlur(D3DXVECTOR3 setPos, float power)
{
	D3DXMATRIX view, projection;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &projection);

	D3DXVec3TransformCoord(&setPos, &setPos, &view);
	D3DXVec3TransformCoord(&setPos, &setPos, &projection);
	TranslateViewPort(&setPos, &setPos);

	float center[] = { setPos.x / SCREEN_WIDTH, setPos.y / SCREEN_HEIGHT };
	effect->SetFloatArray(centerTexel, center, 2);
	effect->SetFloat(blurPower, power);
	effect->CommitChanges();

	active = true;
	animIndex = 0;
	cntFrame = 0;
	SetPostEffectUse(EFFECT_SHOCKBLUR, true);
}