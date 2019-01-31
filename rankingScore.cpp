//=====================================
//
//ランキングスコア処理[rankingScore.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "rankingScore.h"
#include "GUIManager.h"
#include "dataContainer.h"

/**************************************
マクロ定義
***************************************/
#define RANKINGSCORE_SIZE_X			(30.0f)
#define RANKINGSCORE_SIZE_Y			(50.0f)
#define RANKINGSCORE_BASEPOS		(D3DXVECTOR3(500.0f, 250.0f, 0.0f))
#define RANKINGSCORE_OFFSET_X		(900.0f)
#define RANKINGSCORE_OFFSET_Y		(150.0f)
#define RANKINGSCORE_NUM_OFFSET		(-40.0f)
#define RANKINGSCORE_DIV_X			(2)
#define RANKINGSCORE_DIV_Y			(5)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexRankingScore(void);
void SetVertexRankingScore(int rank, float offset);

/**************************************
初期化処理
***************************************/
void InitRankingScore(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		MakeVertexRankingScore();
		initialized = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitRankingScore(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateRankingScore(void)
{

}

/**************************************
描画処理
***************************************/
void DrawRankingScore(void)
{
	DATA_HIGHSCRE *data = GetHighScore();

	for (int cntData = 0; cntData < DATACONTAINER_HIGHSCORE_MAX; cntData++)
	{
		int score = data[cntData].score;
		int digitMax = (score == 0) ? 1 : (int)log10f((float)score) + 1;
		for (int i = 0; i < digitMax; i++, score /= 10)
		{
			int num = score % 10;
			SetVertexRankingScore(cntData, RANKINGSCORE_NUM_OFFSET * i);
			DrawGUINum(GUI_NUMRANKING, num, vtxWk);
		}
	}
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexRankingScore(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

/**************************************
頂点設定処理
***************************************/
void SetVertexRankingScore(int rank, float offset)
{
	int x = rank / RANKINGSCORE_DIV_Y;
	int y = rank % RANKINGSCORE_DIV_Y;

	D3DXVECTOR3 basePos = RANKINGSCORE_BASEPOS + D3DXVECTOR3(RANKINGSCORE_OFFSET_X * x, RANKINGSCORE_OFFSET_Y * y, 0.0f);
	basePos.x += offset;

	vtxWk[0].vtx = basePos + D3DXVECTOR3(-RANKINGSCORE_SIZE_X, -RANKINGSCORE_SIZE_Y, 0.0f);
	vtxWk[1].vtx = basePos + D3DXVECTOR3( RANKINGSCORE_SIZE_X, -RANKINGSCORE_SIZE_Y, 0.0f);
	vtxWk[2].vtx = basePos + D3DXVECTOR3(-RANKINGSCORE_SIZE_X,  RANKINGSCORE_SIZE_Y, 0.0f);
	vtxWk[3].vtx = basePos + D3DXVECTOR3( RANKINGSCORE_SIZE_X,  RANKINGSCORE_SIZE_Y, 0.0f);
}