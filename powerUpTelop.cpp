//=====================================
//
//パワーアップテロップ処理[powerUpTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "powerUpTelop.h"
#include "EasingVector.h"
#include "Easing.h"

#include "input.h"

/**************************************
マクロ定義
***************************************/
#define POWERUPTELOP_SEQUENCE_MAX		(3)
#define POWERUPTELOP_TEX_NAME			"data/TEXTURE/UI/powerUpTelop.png"
#define POWERUPTELOP_TEX_DIVIDE_Y		(2)
#define POWERUPTELOP_TEX_SIZE_X			(200.0f)
#define POWERUPTELOP_TEX_SIZE_Y			(50.0f)
#define POWERUPTELOP_BASEPOS_Y			(650.0f)			

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
//アニメーション開始位置
static const D3DXVECTOR3 StartPos[POWERUPTELOP_SEQUENCE_MAX] = {
	D3DXVECTOR3(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_BASEPOS_Y + 200.0f, 0.0f),
	D3DXVECTOR3(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_BASEPOS_Y, 0.0f),
	D3DXVECTOR3(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_BASEPOS_Y, 0.0f)
};

//アニメーション終了位置
static const D3DXVECTOR3 EndPos[POWERUPTELOP_SEQUENCE_MAX] = {
	D3DXVECTOR3(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_BASEPOS_Y, 0.0f),
	D3DXVECTOR3(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_BASEPOS_Y, 0.0f),
	D3DXVECTOR3(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_BASEPOS_Y - 200.0f, 0.0f)
};

//アニメーション開始アルファ
static const float StartAlpha[POWERUPTELOP_SEQUENCE_MAX] = { 0.0f, 1.0f, 1.0f };

//アニメーション終了アルファ
static const float EndAlpha[POWERUPTELOP_SEQUENCE_MAX] = { 1.0f, 1.0f, 0.0f };

//アニメーション時間
static const int AnimDuration[POWERUPTELOP_SEQUENCE_MAX] = { 30, 60, 30 };

//座標アニメーションタイプ
static const EASINGVECTOR_TYPE PositionAnimType[POWERUPTELOP_SEQUENCE_MAX] = {
	OutCubic_Vector,
	Linear_Vector,
	InCubic_Vector
};

//アルファアニメーションタイプ
static const EASING_TYPE AlphaAnimType[POWERUPTELOP_SEQUENCE_MAX] = {
	OutCubic,
	Linear,
	InCubic
};

static VERTEX_2D vtxWk[NUM_VERTEX];		//頂点情報
static LPDIRECT3DTEXTURE9 texture;		//テクスチャ
static float vtxRadius, vtxAngle;		//各頂点の角度、半径
static int currentSequence;				//現在のシーケンス番号
static int cntFrame;					//カウントフレーム
/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexPowerUpTelop(void);
void SetVertexPowerUpTelop(D3DXVECTOR3 basePos);
void SetTexturePowerUpTelop(int num);
void SetDiffisePowerUpTelop(float alpha);

/**************************************
初期化処理
***************************************/
void InitPowerUpTelop(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (!initialized)
	{
		MakeVertexPowerUpTelop();

		texture = CreateTextureFromFile((LPSTR)POWERUPTELOP_TEX_NAME, pDevice);

		initialized = true;
	}

	SetDiffisePowerUpTelop(0.0f);
	currentSequence = POWERUPTELOP_SEQUENCE_MAX;
}

/**************************************
終了処理
***************************************/
void UninitPowerUpTelop(int num)
{
	SetDiffisePowerUpTelop(0.0f);
	currentSequence = POWERUPTELOP_SEQUENCE_MAX;

	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdatePowerUpTelop(void)
{
	if (currentSequence >= POWERUPTELOP_SEQUENCE_MAX)
		return;

	cntFrame++;

	//キーフレームを計算
	float t = (float)cntFrame / (float)AnimDuration[currentSequence];

	//座標アニメーション
	D3DXVECTOR3 pos = GetEasingValueVector(t, StartPos[currentSequence], EndPos[currentSequence], PositionAnimType[currentSequence]);
	SetVertexPowerUpTelop(pos);

	//アルファアニメーション
	float alpha = GetEasingValue(t, StartAlpha[currentSequence], EndAlpha[currentSequence], AlphaAnimType[currentSequence]);
	SetDiffisePowerUpTelop(alpha);

	if (cntFrame >= AnimDuration[currentSequence])
	{
		currentSequence++;
		cntFrame = 0;
	}

}

/**************************************
描画処理
***************************************/
void DrawPowerUpTelop(void)
{
	if (currentSequence >= POWERUPTELOP_SEQUENCE_MAX)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, texture);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
頂点情報作成処理
***************************************/
void MakeVertexPowerUpTelop(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	vtxAngle = atan2f(POWERUPTELOP_TEX_SIZE_Y, POWERUPTELOP_TEX_SIZE_X);
	vtxRadius = D3DXVec2Length(&D3DXVECTOR2(POWERUPTELOP_TEX_SIZE_X, POWERUPTELOP_TEX_SIZE_Y));
}

/**************************************
頂点設定処理
***************************************/
void SetVertexPowerUpTelop(D3DXVECTOR3 basePos)
{
	vtxWk[0].vtx.x = basePos.x - cosf(vtxAngle) * vtxRadius;
	vtxWk[0].vtx.y = basePos.y - sinf(vtxAngle) * vtxRadius;
	vtxWk[1].vtx.x = basePos.x + cosf(vtxAngle) * vtxRadius;
	vtxWk[1].vtx.y = basePos.y - sinf(vtxAngle) * vtxRadius;
	vtxWk[2].vtx.x = basePos.x - cosf(vtxAngle) * vtxRadius;
	vtxWk[2].vtx.y = basePos.y + sinf(vtxAngle) * vtxRadius;
	vtxWk[3].vtx.x = basePos.x + cosf(vtxAngle) * vtxRadius;
	vtxWk[3].vtx.y = basePos.y + sinf(vtxAngle) * vtxRadius;
}

/**************************************
テクスチャ座標セット処理
***************************************/
void SetTexturePowerUpTelop(int num)
{
	float sizeY = 1.0f / POWERUPTELOP_TEX_DIVIDE_Y;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, num * sizeY);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, num * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, (num + 1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, (num + 1) * sizeY);
}

/**************************************
頂点ディフューズセット処理
***************************************/
void SetDiffisePowerUpTelop(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}

/**************************************
テロップアニメーション開始処理
***************************************/
void StartPowerUpTelopAnimation(int num)
{
	//ショットレベルアップはペンディング
	if (num == 1)
		return;

	currentSequence = 0;
	cntFrame = 0;
	SetTexturePowerUpTelop(num);
}