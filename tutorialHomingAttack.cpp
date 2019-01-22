//=====================================
//
//チュートリアルホーミング攻撃処理[tutorialHomingAttack.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "tutorialController.h"
#include "Easing.h"
#include "playerModel.h"

/**************************************
マクロ定義
***************************************/
#define TUTORIAL_HOMINGATTACK_TEXTURE_NAME		"data/TEXTURE/UI/tutorial00.png"
#define TUTORIAL_HOMINGATTACK_TEX_DIVIDE_Y		(1)
#define TUTORIAL_HOMINGATTACK_TEX_SIZE_X		(400)
#define TUTORIAL_HOMINGATTACK_TEX_SIZE_Y		(200)
#define TUTORIAL_HOMINGATTACK_ANIM_END			(3)
#define TUTORIAL_HOMINGATTACK_ANIM_MAX			(3)	
#define TUTORIAL_HOMINGATTACK_BASEPOS			(D3DXVECTOR3(SCREEN_CENTER_X, 200.0f, 0.0f))
#define TUTORIAL_HOMINGATTACK_CHECKNUM			(3)

static const float AnimStartAlpha[TUTORIAL_HOMINGATTACK_ANIM_MAX] = {
	0.0f, 1.0f, 1.0f,
};

static const float AnimEndAlpha[TUTORIAL_HOMINGATTACK_ANIM_MAX] = {
	1.0f, 1.0f, 0.0f,
};

static const int AnimDuration[TUTORIAL_HOMINGATTACK_ANIM_MAX] = {
	30, 240, 30,
};

static const EASING_TYPE easingType[TUTORIAL_HOMINGATTACK_ANIM_MAX] = {
	OutCubic, Linear, InCubic,
};

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];
static int cntFrame;
static float vtxAngle, vtxRadius;
static int animIndex;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexTutorialHomingAttack(void);
void SetVertexTutorialHomingAttack(D3DXVECTOR3 pos);
void SetTextureTutorialHomingAttack(int num);
void SetDiffuseTutorialHomingAttack(float alpha);

/**************************************
入場処理
***************************************/
void OnEnterTutorialHomingAttack(void)
{
	MakeVertexTutorialHomingAttack();
	cntFrame = 0;
	animIndex = 0;
	ChangeStatePlayerModel(PlayerFPS)
		;
}

/**************************************
更新処理
***************************************/
void OnUpdateTutorialHomingAttack(void)
{
	cntFrame++;
	float t = (float)cntFrame / (float)AnimDuration[animIndex];
	float alpha = GetEasingValue(t, AnimStartAlpha[animIndex], AnimEndAlpha[animIndex], easingType[animIndex]);
	SetDiffuseTutorialHomingAttack(alpha);
	SetVertexTutorialHomingAttack(TUTORIAL_HOMINGATTACK_BASEPOS);
	SetTextureTutorialHomingAttack(animIndex / TUTORIAL_HOMINGATTACK_ANIM_END);

	if (cntFrame >= AnimDuration[animIndex])
	{
		if (animIndex == TUTORIAL_HOMINGATTACK_ANIM_END - 2)
		{
			if (GetTutorialHomingAttackNum() >= TUTORIAL_HOMINGATTACK_CHECKNUM)
			{
				cntFrame = 0;
				animIndex++;
			}
		}
		else
		{
			cntFrame = 0;
			animIndex++;
		}

		if (animIndex == TUTORIAL_HOMINGATTACK_ANIM_MAX)
		{
			ChangeStateTutorialController(TutorialHardCube);
		}
	}
}

/**************************************
描画処理
***************************************/
void OnDrawTutorialHomingAttack(LPDIRECT3DTEXTURE9 tex)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, tex);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexTutorialHomingAttack(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxRadius = D3DXVec2Length(&D3DXVECTOR2(TUTORIAL_HOMINGATTACK_TEX_SIZE_X, TUTORIAL_HOMINGATTACK_TEX_SIZE_Y));
	vtxAngle = atan2f(TUTORIAL_HOMINGATTACK_TEX_SIZE_Y, TUTORIAL_HOMINGATTACK_TEX_SIZE_X);
}

/**************************************
頂点座標設定処理
***************************************/
void SetVertexTutorialHomingAttack(D3DXVECTOR3 pos)
{
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
テクスチャ座標設定処理
***************************************/
void SetTextureTutorialHomingAttack(int num)
{
	float sizeY = 1.0f / TUTORIAL_HOMINGATTACK_TEX_DIVIDE_Y;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, num * sizeY);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, num * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, (num + 1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, (num + 1) * sizeY);
}

/**************************************
ディフューズ設定処理
***************************************/
void SetDiffuseTutorialHomingAttack(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}