//=====================================
//
//シーンフェード処理[sceneFade.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "sceneFade.h"

/**************************************
マクロ定義
***************************************/
#define SCENEFADE_INOUT_DURATION		(60)
#define SCENEFADE_WAIT_DURATION			(30)

/**************************************
構造体定義
***************************************/
typedef void(*funcSceneFade)(void);

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexSceneFade(void);
void SetDiffuseSceneFade(float alpha);

void UpdateSceneFadeIn(void);
void UpdateSceneFadeWait(void);
void UpdateSceneFadeOut(void);
void UpdateSceneFadeNone(void);

/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];
static int cntFrame;
static DefineScene nextScene;
static SceneFadeState currentState;

//更新処理の関数テーブル
static funcSceneFade Update[SceneFadeStateMax]
{
	UpdateSceneFadeIn,
	UpdateSceneFadeWait,
	UpdateSceneFadeOut,
	UpdateSceneFadeNone
};

/**************************************
初期化処理
***************************************/
void InitSceneFade(int num)
{
	MakeVertexSceneFade();
	ChangeStateSceneFade(SceneFadeNone);
}

/**************************************
終了処理
***************************************/
void UninitSceneFade(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateSceneFade(void)
{
	Update[currentState]();
}

/**************************************
描画処理
***************************************/
void DrawSceneFade(void)
{
	if (currentState == SceneFadeNone)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
フェード状態変更処理
***************************************/
void ChangeStateSceneFade(SceneFadeState state)
{
	currentState = state;
	cntFrame = 0;
}

/**************************************
シーンフェードセット処理
***************************************/
void SetSceneFade(DefineScene next)
{
	nextScene = next;
	ChangeStateSceneFade(SceneFadeIn);
}

/**************************************
フェードイン処理
***************************************/
void UpdateSceneFadeIn(void)
{
	cntFrame++;

	float alpha = (float)cntFrame / SCENEFADE_INOUT_DURATION;
	SetDiffuseSceneFade(alpha);

	if (cntFrame == SCENEFADE_INOUT_DURATION)
		ChangeStateSceneFade(SceneFadeWait);
}

/**************************************
フェードウェイト処理
***************************************/
void UpdateSceneFadeWait(void)
{
	cntFrame++;

	SetDiffuseSceneFade(1.0f);

	if (cntFrame == SCENEFADE_WAIT_DURATION)
	{
		SetScene(nextScene);
		ChangeStateSceneFade(SceneFadeOut);
	}

}

/**************************************
フェードアウト処理
***************************************/
void UpdateSceneFadeOut(void)
{
	cntFrame++;
	float alpha = (float)cntFrame / SCENEFADE_INOUT_DURATION;
	alpha = 1.0f - alpha;
	SetDiffuseSceneFade(alpha);

	if (cntFrame == SCENEFADE_INOUT_DURATION)
		ChangeStateSceneFade(SceneFadeNone);

}

/**************************************
フェード無し処理
***************************************/
void UpdateSceneFadeNone(void)
{
	//何もしない
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexSceneFade(void)
{
	vtxWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtxWk[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vtxWk[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vtxWk[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;
}

/**************************************
ディフューズ設定処理
***************************************/
void SetDiffuseSceneFade(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}