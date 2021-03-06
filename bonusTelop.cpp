//=====================================
//
//ボーナステロップ処理[bonusTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bonusTelop.h"
#include "EasingVector.h"
#include "bonusTimeGUI.h"
#include "soundEffectManager.h"

/**************************************
マクロ定義
***************************************/
#define BONUSTELOP_TEX_MAX				(2)
#define BONUSTELOP_TEX_DIVIDE_Y			(2)
#define BONUSTELOP_TEX_SIZE_X			(400)
#define BONUSTELOP_TEX_SIZE_Y			(100)
#define BONUSTELOP_DISP_POS				(D3DXVECTOR3(SCREEN_CENTER_X, 300.0f, 0.0f))
#define BONUSTELOP_SEQUENCE_MAX			(3)
#define BONUSTELOP_SET_COUNT			(600)
#define BONUSTELOP_PLAYSE_TIMING		(1)

//アニメーション開始位置
static const D3DXVECTOR3 StartOffset[BONUSTELOP_SEQUENCE_MAX] = {
	D3DXVECTOR3(SCREEN_CENTER_X + BONUSTELOP_TEX_SIZE_X, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
};

//アニメーション終了位置
static const D3DXVECTOR3 EndOffset[BONUSTELOP_SEQUENCE_MAX] = {
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-SCREEN_CENTER_X - BONUSTELOP_TEX_SIZE_X, 0.0f, 0.0f)
};

//アニメーションタイプ
static const EASINGVECTOR_TYPE AnimType[BONUSTELOP_SEQUENCE_MAX] = {
	OutExponential_Vector,
	Linear_Vector,
	InExponential_Vector
};

//アニメーション時間
static const int AnimDuration[BONUSTELOP_SEQUENCE_MAX] = {
	30, 60, 30
};

//テクスチャパス
static const char* TexturePath[BONUSTELOP_TEX_MAX] = {
	"data/TEXTURE/UI/bonusTelop.png",
	"data/TEXTURE/UI/bonusTelopEnd.png"
};
/**************************************
構造体定義
***************************************/
enum BONUSTELOP_DEFINE
{
	BonusTelopUpper,
	BonusTelopLower,
	BonusTelopDefineEnd
};

/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];
static LPDIRECT3DTEXTURE9 texture[BONUSTELOP_TEX_MAX];
static D3DXVECTOR3 offsetPos;
static float vtxAngle, vtxRadius;
static int animIndex, cntFrame;
static bool isStartTelop;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexBonusTelop(void);
void SetVertexBonusTelop(BONUSTELOP_DEFINE define);
void SetTextureBonusTelop(BONUSTELOP_DEFINE define);

/**************************************
初期化処理
***************************************/
void InitBonusTelop(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		for (int i = 0; i < BONUSTELOP_TEX_MAX; i++)
			texture[i] = CreateTextureFromFile((LPSTR)TexturePath[i], pDevice);

		MakeVertexBonusTelop();

		initialized = true;
	}

	animIndex = BONUSTELOP_SEQUENCE_MAX;
	cntFrame = 0;
	isStartTelop = true;
}

/**************************************
終了処理
***************************************/
void UninitBonusTelop(int num)
{
	animIndex = BONUSTELOP_SEQUENCE_MAX;
	cntFrame = 0;

	if (num == 0)
	{
		for (int i = 0; i < BONUSTELOP_TEX_MAX; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateBonusTelop(void)
{
	if (animIndex >= BONUSTELOP_SEQUENCE_MAX)
		return;

	cntFrame++;

	//移動アニメーション
	float t = (float)cntFrame / (float)AnimDuration[animIndex];
	offsetPos = GetEasingValueVector(t, StartOffset[animIndex], EndOffset[animIndex], AnimType[animIndex]);

	//遷移判定
	if (cntFrame == AnimDuration[animIndex])
	{
		cntFrame = 0;
		animIndex++;

		if (animIndex == BONUSTELOP_SEQUENCE_MAX && isStartTelop)
			StartBonusTimeCount(BONUSTELOP_SET_COUNT);
	}
}

/**************************************
描画処理
***************************************/
void DrawBonusTelop(void)
{
	if (animIndex >= BONUSTELOP_SEQUENCE_MAX)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPDIRECT3DTEXTURE9 tex = (isStartTelop) ? texture[0] : texture[1];
	pDevice->SetTexture(0, tex);
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テロップ上部の描画
	SetTextureBonusTelop(BonusTelopUpper);
	SetVertexBonusTelop(BonusTelopUpper);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));

	//テロップ下部の描画
	SetTextureBonusTelop(BonusTelopLower);
	SetVertexBonusTelop(BonusTelopLower);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));

}

/**************************************
頂点作成処理
***************************************/
void MakeVertexBonusTelop(void)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxAngle = atan2f(BONUSTELOP_TEX_SIZE_Y, BONUSTELOP_TEX_SIZE_X);
	vtxRadius = D3DXVec2Length(&D3DXVECTOR2(BONUSTELOP_TEX_SIZE_X, BONUSTELOP_TEX_SIZE_Y));
}

/**************************************
頂点座標設定処理
***************************************/
void SetVertexBonusTelop(BONUSTELOP_DEFINE define)
{
	D3DXVECTOR3 pos = BONUSTELOP_DISP_POS;
	pos += (define == BonusTelopUpper) ? -offsetPos : offsetPos;
	pos.y += (define == BonusTelopUpper) ? -BONUSTELOP_TEX_SIZE_Y : BONUSTELOP_TEX_SIZE_Y;

	vtxWk[0].vtx.x = pos.x - cosf(vtxAngle) * vtxRadius;
	vtxWk[0].vtx.y = pos.y - sinf(vtxAngle) * vtxRadius;
	vtxWk[1].vtx.x = pos.x + cosf(vtxAngle) * vtxRadius;
	vtxWk[1].vtx.y = pos.y - sinf(vtxAngle) * vtxRadius;
	vtxWk[2].vtx.x = pos.x - cosf(vtxAngle) * vtxRadius;
	vtxWk[2].vtx.y = pos.y + sinf(vtxAngle) * vtxRadius;
	vtxWk[3].vtx.x = pos.x + cosf(vtxAngle) * vtxRadius;
	vtxWk[3].vtx.y = pos.y + sinf(vtxAngle) * vtxRadius;
}

/**************************************
UV座標設定処理
***************************************/
void SetTextureBonusTelop(BONUSTELOP_DEFINE define)
{
	float sizeY = 1.0f / BONUSTELOP_TEX_DIVIDE_Y;
	int y = (define == BonusTelopUpper) ? 0 : 1;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, y * sizeY);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, y * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, (y + 1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, (y + 1) * sizeY);
}

/**************************************
アニメーション開始処理
***************************************/
void StartBonusTelopAnim(bool isStart)
{
	animIndex = 0;
	cntFrame = 0;
	isStartTelop = isStart;

	//開始テロップのみSE再生
	if(isStart)
		PlaySE(DefineSE::BONUSSTART);
}