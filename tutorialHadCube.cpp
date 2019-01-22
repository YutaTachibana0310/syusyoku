//=====================================
//
//チュートリアルハードキューブ処理[tutorialHardCube.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "tutorialController.h"
#include "Easing.h"
#include "hardCubeObject.h"

/**************************************
マクロ定義
***************************************/
#define TUTORIAL_HARDCUBE_TEXTURE_NAME		"data/TEXTURE/UI/tutorial00.png"
#define TUTORIAL_HARDCUBE_TEX_DIVIDE_Y		(2)
#define TUTORIAL_HARDCUBE_TEX_SIZE_X		(400)
#define TUTORIAL_HARDCUBE_TEX_SIZE_Y		(200)
#define TUTORIAL_HARDCUBE_ANIM_END			(4)
#define TUTORIAL_HARDCUBE_ANIM_MAX			(8)	
#define TUTORIAL_HARDCUBE_BASEPOS			(D3DXVECTOR3(SCREEN_CENTER_X, 200.0f, 0.0f))
#define TUTORIAL_HARDCUBE_CUBE_MAX			(4)
#define TUTORIAL_HARDCUBE_EMIITT			(300)

static const float AnimStartAlpha[TUTORIAL_HARDCUBE_ANIM_MAX] = {
	0.0f, 1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f
};

static const float AnimEndAlpha[TUTORIAL_HARDCUBE_ANIM_MAX] = {
	1.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 0.0f,
};

static const int AnimDuration[TUTORIAL_HARDCUBE_ANIM_MAX] = {
	30, 300, 30, 60,
	30, 300, 30, 240
};

static const EASING_TYPE easingType[TUTORIAL_HARDCUBE_ANIM_MAX] = {
	OutCubic, Linear, InCubic, OutCubic, Linear, InCubic,
};

//キューブ生成位置
static const D3DXVECTOR3 CubeInitPos[TUTORIAL_HARDCUBE_CUBE_MAX] = {
	D3DXVECTOR3(-100.0f, 100.0f, -100.0f),
	D3DXVECTOR3(100.0f, 100.0f, -100.0f),
	D3DXVECTOR3(-100.0f, -100.0f, -100.0f),
	D3DXVECTOR3(100.0f, -100.0f, -100.0f),
};

//キューブ目標位置
static const D3DXVECTOR3 CubeGOalPos[TUTORIAL_HARDCUBE_CUBE_MAX] = {
	D3DXVECTOR3(-100.0f, 50.0f, 600.0f),
	D3DXVECTOR3(100.0f, 50.0f, 600.0f),
	D3DXVECTOR3(-100.0f, -150.0f, 600.0f),
	D3DXVECTOR3(100.0f, -150.0f, 600.0f),
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
static int cubeType;

static HARD_CUBE_OBJECT *cubePtr[TUTORIAL_HARDCUBE_CUBE_MAX];

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexTutorialHardCube(void);
void SetVertexTutorialHardCube(D3DXVECTOR3 pos);
void SetTextureTutorialHardCube(int num);
void SetDiffuseTutorialHardCube(float alpha);
bool CheckTutorialCubeAlive(void);

/**************************************
入場処理
***************************************/
void OnEnterTutorialHardCube(void)
{
	MakeVertexTutorialHardCube();
	cntFrame = 0;
	animIndex = 0;
	ZeroMemory(cubePtr, sizeof(cubePtr));

	for (int i = 0; i < TUTORIAL_HARDCUBE_CUBE_MAX; i++)
	{
		cubePtr[i] = SetHardCubeObjectDirectData(CubeInitPos[i], CubeGOalPos[i], HardCubeTutorialType, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

/**************************************
更新処理
***************************************/
void OnUpdateTutorialHardCube(void)
{

	cntFrame++;
	float t = (float)cntFrame / (float)AnimDuration[animIndex];
	float alpha = GetEasingValue(t, AnimStartAlpha[animIndex], AnimEndAlpha[animIndex], easingType[animIndex]);
	SetDiffuseTutorialHardCube(alpha);
	SetVertexTutorialHardCube(TUTORIAL_HARDCUBE_BASEPOS);
	SetTextureTutorialHardCube(animIndex / TUTORIAL_HARDCUBE_ANIM_END);

	if (cntFrame == AnimDuration[animIndex])
	{
		animIndex++;
		cntFrame = 0;
		if (animIndex == TUTORIAL_HARDCUBE_ANIM_MAX)
		{
			ChangeStateTutorialController(TutorialBonusCube);
		}
	}

	if (animIndex == TUTORIAL_HARDCUBE_ANIM_MAX - 1 && !CheckTutorialCubeAlive())
	{
		ChangeStateTutorialController(TutorialBonusCube);
	}
}

/**************************************
描画処理
***************************************/
void OnDrawTutorialHardCube(LPDIRECT3DTEXTURE9 tex)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, tex);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexTutorialHardCube(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxRadius = D3DXVec2Length(&D3DXVECTOR2(TUTORIAL_HARDCUBE_TEX_SIZE_X, TUTORIAL_HARDCUBE_TEX_SIZE_Y));
	vtxAngle = atan2f(TUTORIAL_HARDCUBE_TEX_SIZE_Y, TUTORIAL_HARDCUBE_TEX_SIZE_X);
}

/**************************************
頂点座標設定処理
***************************************/
void SetVertexTutorialHardCube(D3DXVECTOR3 pos)
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
void SetTextureTutorialHardCube(int num)
{
	float sizeY = 1.0f / TUTORIAL_HARDCUBE_TEX_DIVIDE_Y;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, num * sizeY);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, num * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, (num + 1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, (num + 1) * sizeY);
}

/**************************************
ディフューズ設定処理
***************************************/
void SetDiffuseTutorialHardCube(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}

/**************************************
キューブの生存確認
***************************************/
bool CheckTutorialCubeAlive(void)
{
	for (int i = 0; i < TUTORIAL_HARDCUBE_CUBE_MAX; i++)
	{
		if (!cubePtr[i]->active)
			return false;
	}

	return true;
}