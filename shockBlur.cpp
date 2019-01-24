//=====================================
//
//衝撃ブラー処理[shockBlur.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "shockBlur.h"
#include <assert.h>
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define SHOCKBLUR_EFFECT_NAME	"data/EFFECT/shockBlur.fx"
#define FVF_SCREEN_SQUARE		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/**************************************
構造体定義
***************************************/
typedef struct {
	D3DXVECTOR3 vtx;
	float rhw;
	D3DXCOLOR color;
	D3DXVECTOR2 tex;
}VTX_SCREEN_SQUARE;

/**************************************
グローバル変数
***************************************/
static LPD3DXEFFECT effect = NULL;							//シェーダ
static D3DXHANDLE tech, centerTexel, tU, tV, blurPower;		//シェーダ内グローバル変数
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitShcokBlur(int num)
{
	static bool initialized = false;

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

	effect->SetTechnique(tech);

	pDevice->CreateVertexBuffer(sizeof(VTX_SCREEN_SQUARE) * NUM_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_SCREEN_SQUARE,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL);

	VTX_SCREEN_SQUARE *pVtx = NULL;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3((float)SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f / (float)SCREEN_WIDTH, 0.0f + 0.5f / (float)SCREEN_HEIGHT);
	pVtx[1].tex = D3DXVECTOR2(1.0f + 0.5f / (float)SCREEN_WIDTH, 0.0f + 0.5f / (float)SCREEN_HEIGHT);
	pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f / (float)SCREEN_WIDTH, 1.0f + 0.5f / (float)SCREEN_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(1.0f + 0.5f / (float)SCREEN_WIDTH, 1.0f + 0.5f / (float)SCREEN_HEIGHT);

	pVtx[0].color =
		pVtx[1].color =
		pVtx[2].color =
		pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxBuff->Unlock();

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

}

/**************************************
描画処理
***************************************/
void DrawShcokBlur(void)
{
	static float power = 5.0f;
	static D3DXVECTOR2 center = D3DXVECTOR2(SCREEN_CENTER_X, SCREEN_CENTER_Y);

	BeginDebugWindow("ShockBlur");
	DebugSliderFloat("power", &power, 0.0f, 100.0f);
	DebugSliderFloat("centerX", &center.x, 0.0f, SCREEN_WIDTH);
	DebugSliderFloat("centerY", &center.y, 0.0f, SCREEN_HEIGHT);
	EndDebugWindow("ShockBlur");

	effect->SetFloat(blurPower, power);
	effect->SetFloatArray(centerTexel, (float*)&center, 2);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	effect->Begin(NULL, 0);
	effect->BeginPass(0);

	pDevice->SetFVF(FVF_SCREEN_SQUARE);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VTX_SCREEN_SQUARE));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	effect->EndPass();
	effect->End();
}
