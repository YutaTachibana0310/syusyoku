//=====================================
//
//バトルコントローラ処理[battleController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"
#include "fuzzyLogic.h"
#include "playerModel.h"
#include "debugWindow.h"
#include "battleCamera.h"
#include "enemyManager.h"

/**************************************
マクロ定義
***************************************/
#define BATTLE_SPACE_DIVIDE_NUM			(4)					//エネミー生成範囲分割数
#define BATTLE_SPACE_LEFT_BORDER		(-300.0f)			//エネミーの生成座標範囲（左端）
#define BATTLE_SPACE_TOP_BORDER			(300.0f)			//エネミーの生成座標範囲（上端）
#define BATTLE_SPACE_RIGHT_BORDER		(300.0f)			//エネミーの生成座標範囲（右端）
#define BATTLE_SPACE_BOTTOM_BORDER		(-300.0f)			//エネミーの生成座標範囲（下端）
#define BATTLE_EMITTPOS_Z				(2500.0f)			//エネミー生成座標Z値

#define BATTLE_FUZZY_NEAR_BORDER		(0.0f)				//距離に関するファジィ理論のしきい値1
#define BATTLE_FUZZY_MIDDLE_BORDER		(SCREEN_HEIGHT)		//距離に関するファジィ理論のしきい値2
#define BATTLE_FUZZY_FAR_BORDER			(SCREEN_WIDTH*2.5f)	//距離に関するファジィ理論のしきい値3
#define BATTLE_FUZZY_RECENTLY_BORDER	(120.0f)			//時間に関するファジィ理論のしきい値1
#define BATTLE_FUZZY_LATELY_BORDER		(1200.0f)			//時間に関するファジィ理論のしきい値2

#define BATTLE_SPACE_MAX (BATTLE_SPACE_DIVIDE_NUM*BATTLE_SPACE_DIVIDE_NUM)	//分割された空間の総数

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static D3DXVECTOR2 enemyEmittPos[BATTLE_SPACE_MAX];	//エネミー生成座標
static DWORD lastEmittFrame[BATTLE_SPACE_MAX];		//各空間にエネミーが生成された最後のフレーム
static DWORD cntFrame;								//フレームカウント
static D3DXVECTOR3 checkPos[BATTLE_SPACE_MAX];		//判定座標
static D3DXVECTOR3 emmittPos[BATTLE_SPACE_MAX];		//エネミー生成位置
/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitBattleController(int num)
{
	if (num == 0)
	{
		//生成範囲を分割してエネミーの生成座標を計算
		float spaceUnitWidth = SCREEN_WIDTH / BATTLE_SPACE_DIVIDE_NUM;
		float spaceUnitHeight = SCREEN_HEIGHT / BATTLE_SPACE_DIVIDE_NUM;

		for (int y = 0; y < BATTLE_SPACE_DIVIDE_NUM; y++)
		{
			for (int x = 0; x < BATTLE_SPACE_DIVIDE_NUM; x++)
			{
				checkPos[y * BATTLE_SPACE_DIVIDE_NUM + x].x = spaceUnitWidth * (x + 0.5f);
				checkPos[y * BATTLE_SPACE_DIVIDE_NUM + x].y = spaceUnitHeight * (y + 0.5f);

				emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].x = (BATTLE_SPACE_RIGHT_BORDER - BATTLE_SPACE_LEFT_BORDER) / BATTLE_SPACE_DIVIDE_NUM * (x + 0.5f) + BATTLE_SPACE_LEFT_BORDER;
				emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].y = (BATTLE_SPACE_TOP_BORDER - BATTLE_SPACE_BOTTOM_BORDER) / BATTLE_SPACE_DIVIDE_NUM * (y + 0.5f) + BATTLE_SPACE_BOTTOM_BORDER;
				emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].z = 5000.0f;
			}
		}
	}

	//各パラメータを初期化
	cntFrame = 0;
	for (int i = 0; i < BATTLE_SPACE_MAX; i++)
	{
		lastEmittFrame[i] = 0xffffffff;
	}
}

/**************************************
終了処理
***************************************/
void UninitBattleController(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateBattleController(void)
{
	static float valueLength[BATTLE_SPACE_MAX];
	static float valueTime[BATTLE_SPACE_MAX];
	static float fuzzyValue[BATTLE_SPACE_MAX];
	static int decidedPos;

	D3DXVECTOR3 playerPos;
	float maxValue = -9999.9f;

	cntFrame++;

	D3DXVec3TransformCoord(&playerPos, &GetPlayerAdr(0)->pos, &GetBattleCameraView());
	D3DXVec3TransformCoord(&playerPos, &playerPos, &GetBattleCameraProjection());
	TranslateViewPort(&playerPos, &playerPos);

	//各生成座標に対してファジィ理論で重みを計算
	for (int i = 0; i < BATTLE_SPACE_MAX; i++)
	{
		float length = D3DXVec2Length(&(D3DXVECTOR2)(playerPos - checkPos[i]));
		float elapsedTime = (float)(cntFrame - lastEmittFrame[i]);

		valueLength[i] = fFuzzyTriangle(length, BATTLE_FUZZY_NEAR_BORDER, BATTLE_FUZZY_MIDDLE_BORDER, BATTLE_FUZZY_FAR_BORDER);
		valueTime[i] = fFuzzyRightGrade(elapsedTime, BATTLE_FUZZY_RECENTLY_BORDER, BATTLE_FUZZY_LATELY_BORDER);
		fuzzyValue[i] = valueLength[i] * valueTime[i];
		if (maxValue < fuzzyValue[i])
		{
			decidedPos = i;
			maxValue = fuzzyValue[i];
		}
	}

	//重みが一番大きかった座標へエネミーを生成
	if (cntFrame % 10 == 0)
	{
		lastEmittFrame[decidedPos] = cntFrame;
		EmmittCubeObject(10, &emmittPos[decidedPos]);
	}

	//デバッグ情報表示
	{
		ImGui::Begin("BattleController");

		ImGui::Text("PlayerPos : %f, %f", playerPos.x, playerPos.y);

		ImGui::NewLine();
		ImGui::Text("Length");
		for (int i = 0; i < BATTLE_SPACE_MAX; i++)
		{
			if (i % BATTLE_SPACE_DIVIDE_NUM != 0)
				ImGui::SameLine();
			ImGui::Text("%4f ", valueLength[i]);
		}
		for (int i = 0; i < BATTLE_SPACE_DIVIDE_NUM; i++)
			ImGui::PlotHistogram("", &valueLength[i * BATTLE_SPACE_DIVIDE_NUM], BATTLE_SPACE_DIVIDE_NUM);

		ImGui::NewLine();
		ImGui::Text("Time");
		for (int i = 0; i < BATTLE_SPACE_MAX; i++)
		{
			if (i % BATTLE_SPACE_DIVIDE_NUM != 0)
				ImGui::SameLine();
			ImGui::Text("%4f ", valueTime[i]);
		}
		for (int i = 0; i < BATTLE_SPACE_DIVIDE_NUM; i++)
			ImGui::PlotHistogram("", &valueTime[i * BATTLE_SPACE_DIVIDE_NUM], BATTLE_SPACE_DIVIDE_NUM);

		ImGui::NewLine();
		for (int i = 0; i < BATTLE_SPACE_DIVIDE_NUM; i++)
			ImGui::PlotHistogram("FuzzyValues",
				&fuzzyValue[i * BATTLE_SPACE_DIVIDE_NUM],
				BATTLE_SPACE_DIVIDE_NUM);

		ImGui::End();

	}
}

/**************************************
描画処理
***************************************/
void DrawBattleController(void)
{

}
