//=====================================
//
//GUIマネージャ処理[GUIManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "GUIManager.h"
#include "sceneManager.h"
#include "scoreGUI.h"
#include "hpGUI.h"
#include "lockonGUI.h"
#include "powerUpTelop.h"
#include "bonusTelop.h"
#include "bonusTimeGUI.h"
#include "lockonLevelGUI.h"
#include "lockonNumGUI.h"
#include "gameoverTelop.h"
#include "stageClearTelop.h"
#include "nameEntryRanking.h"
#include "rankingScore.h"
#include "rankingName.h"
#include "nameEntryCursor.h"
#include "rankingTelop.h"
#include "tutorialController.h"
#include "titleTelop.h"
#include "entryTelop.h"
#include "bonusPositinoTelop.h"
#include "scoreMagniNumGUI.h"
#include "scoreMagniGauge.h"
#include "numGUI.h"
#include "playerModel.h"
#include "Easing.h"
#include "cautionTelop.h"

#include "logoScene.h"
#include "titleScene.h"
#include "sceneFade.h"
#include <stdio.h>
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define GUI_NUMTEX_DIVIDE_X				(5)
#define GUI_NUMTEX_DIVIDE_Y				(2)
#define GUI_NUMTEX_PATTERN_MAX			(10)

/**************************************
構造体定義
***************************************/
typedef void(*FuncGUI)(void);

/**************************************
プロトタイプ宣言
***************************************/
//入場処理テーブル
void OnEnterLogoSceneGUI(void);
void OnEnterTitleSceneGUI(void);
void OnEnterTutorialSceneGUI(void);
void OnEnterBattleSceneGUI(void);
void OnEnterGameoverSceneGUI(void);
void OnEnterStageclearSceneGUI(void);
void OnEnterNameEntrySceneGUI(void);

//更新処理テーブル
void OnUpdateLogoSceneGUI(void);
void OnUpdateTitleSceneGUI(void);
void OnUpdateTutorialSceneGUI(void);
void OnUpdateBattleSceneGUI(void);
void OnUpdateGameoverSceneGUI(void);
void OnUpdateStageclearSceneGUI(void);
void OnUpdateNameEntrySceneGUI(void);

//描画処理テーブル
void OnDrawLogoSceneGUI(void);
void OnDrawTitleSceneGUI(void);
void OnDrawTutorialSceneGUI(void);
void OnDrawBattleSceneGUI(void);
void OnDrawGameoverSceneGUI(void);
void OnDrawStageclearSceneGUI(void);
void OnDrawNameEntrySceneGUI(void);

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture[GUI_NUMTEX_MAX];
static const char* texturePath[GUI_NUMTEX_MAX] =
{
	"data/TEXTURE/UI/scoreNum.png",
	"data/TEXTURE/UI/lockonNum.png",
	"data/TEXTURE/UI/timeNum.png",
	"data/TEXTURE/UI/rankingNum.png"
};

//各シーンのGUI入場処理テーブル
static FuncGUI Init[DefineSceneMax] = {
	OnEnterLogoSceneGUI,//EnterGUIonLogoScene,
	OnEnterTitleSceneGUI,//EnterGUIonTitleScene,
	OnEnterBattleSceneGUI,//EnterGUIonBattleScene,
	NULL,
	OnEnterTutorialSceneGUI,//EnterGUIonTutorialScene,
	OnEnterGameoverSceneGUI,//EnterGUIonGameoverScene,
	OnEnterStageclearSceneGUI,//EnterGUIonStageClearScene,
	OnEnterNameEntrySceneGUI//EnterGUIonNameEntryScene
};

//各シーンのGUI更新処理テーブル
static FuncGUI Update[DefineSceneMax] = {
	OnUpdateLogoSceneGUI,//UpdateGUIonLogoScene,
	OnUpdateTitleSceneGUI,//UpdateGUIonTitleScene,
	OnUpdateBattleSceneGUI,//UpdateGUIonBattleScene,
	NULL,
	OnUpdateTutorialSceneGUI,//UpdateGUIonTutorialScene,
	OnUpdateGameoverSceneGUI,//UpdateGUIonGameoverScene,
	OnUpdateStageclearSceneGUI,//UpdateGUIonStageClearScene,
	OnUpdateNameEntrySceneGUI//UpdateGUIonNameEntryScene
};

//各シーンのGUI描画処理テーブル
static FuncGUI Draw[DefineSceneMax] = {
	OnDrawLogoSceneGUI,//DrawGUIonLogoScene,
	OnDrawTitleSceneGUI,//DrawGUIonTitleScene,
	OnDrawBattleSceneGUI,//DrawGUIonBattleScene,
	NULL,
	OnDrawTutorialSceneGUI,//DrawGUIonTutorialScene,
	OnDrawGameoverSceneGUI,//DrawGUIonGameoverScene,
	OnDrawStageclearSceneGUI,//DrawGUIonStageClearScene,
	OnDrawNameEntrySceneGUI//DrawGUIonNameEntryScene
};

NumGUI* NumGUI::instance = nullptr;

/**************************************
初期化処理
***************************************/
void InitGUIManager(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (!initialized)
	{
		for (int i = 0; i < GUI_NUMTEX_MAX; i++)
		{
			texture[i] = CreateTextureFromFile((LPSTR)texturePath[i], pDevice);
		}
		NumGUI::Create();
		initialized = true;
	}

	int scene = GetCurrentScene();
	Init[scene]();
	InitSceneFade(num);

	//InitScoreGUI(num);
	//InitHpGUI(num);
	//InitLockonGUI(num);
	//InitPowerUpTelop(num);
	//InitBonusTelop(num);
	//InitBonusTimeGUI(num);
	//InitSceneFade(num);
	//InitLockonLevelGUI(num);
	//InitLockonNumGUI(num);
	//InitGameoverTelop(num);
	//InitStageClearTelop(num);
	//InitNameEntryRanking(num);
	//InitRankingScore(num);
	//InitRankingName(num);
	//InitNameEntryCursor(num);
	//InitRankingTelop(num);
	//InitTitleTelop(num);
	//InitEntryTelop(num);
	//InitBonusPositionTelop(num);
	//InitScoreMagniNumGUI(num);
	//InitScoreMagniGauge(num);
	//InitCautionTelop(num);
	//LoadSettingsGUI();
}

/**************************************
終了処理
***************************************/
void UninitGUIManager(int num)
{
	if (num == 0)
	{
		for (int i = 0; i < GUI_NUMTEX_MAX; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
	}

	NumGUI::Release();

	UninitScoreGUI(num);
	UninitHpGUI(num);
	UninitLockonGUI(num);
	UninitPowerUpTelop(num);
	UninitBonusTelop(num);
	UninitBonusTimeGUI(num);
	UninitSceneFade(num);
	UninitLockonLevelGUI(num);
	UninitLockonNumGUI(num);
	UninitGameoverTelop(num);
	UninitStageClearTelop(num);
	UninitNameEntryRanking(num);
	UninitRankingScore(num);
	UninitRankingName(num);
	UninitNameEntryCursor(num);
	UninitRankingTelop(num);
	UninitTitleTelop(num);
	UninitEntryTelop(num);
	UninitBonusPositionTelop(num);
	UninitScoreMagniNumGUI(num);
	UninitScoreMagniGauge(num);
	UninitCautionTelop(num);
}

/**************************************
更新処理
***************************************/
void UpdateGUIManager(void)
{
	int scene = GetCurrentScene();
	Update[scene]();
	UpdateSceneFade();

	//UpdateScoreGUI();
	//UpdateHpGUI();
	//UpdateLockonGUI();
	//UpdatePowerUpTelop();
	//UpdateBonusTelop();
	//UpdateBonusTimeGUI();
	//UpdateSceneFade();
	//UpdateLockonLevelGUI();
	//UpdateLockonNumGUI();
	//UpdateGameoverTelop();
	//UpdateStageClearTelop();
	//UpdateNameEntryRanking();
	//UpdateRankingScore();
	//UpdateRankingName();
	//UpdateNameEntryCursor();
	//UpdateRankingTelop();
	//UpdateTitleTelop();
	//UpdateEntryTelop();
	//UpdateBonusPositionTelop();
	//UpdateScoreMagniNumGUI();
	//UpdateScoreMagniGauge();
	//UpdateCautionTelop();
}

/**************************************
描画処理
***************************************/
void DrawGUIManager(void)
{
	int scene = GetCurrentScene();
	if (Draw[scene] != NULL)
	{
		Draw[scene]();
	}

	DrawSceneFade();
}

/**************************************
ロゴシーンGUI描画処理
***************************************/
void DrawGUIonLogoScene(void)
{
	DrawCircleLogo();
}

/**************************************
タイトルシーンGUI描画処理
***************************************/
void DrawGUIonTitleScene(void)
{
	DrawTitleLogo();
	DrawTitleTelop();
}

/**************************************
チュートリアルシーン描画処理
***************************************/
void DrawGUIonTutorialScene(void)
{
	DrawLockonGUI();
	DrawTutorialController();
}

/**************************************
バトルシーンGUI描画処理
***************************************/
void DrawGUIonBattleScene(void)
{
	PLAYERMODEL *player = GetPlayerAdr(0);
	D3DXVECTOR3 screenPos;
	D3DXMATRIX view, proj;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	D3DXVec3TransformCoord(&screenPos, &player->pos, &view);
	D3DXVec3TransformCoord(&screenPos, &screenPos, &proj);
	TranslateViewPort(&screenPos, &screenPos);

	float alpha = EaseLinear((screenPos.x - 100.0f) / 600.0f, 0.2f, 1.0f);

	DrawLockonGUI();

	NumGUI::GetInstance()->SetAlpha(alpha);
	DrawScoreGUI(alpha);
	DrawHpGUI(alpha);
	DrawScoreMagniNumGUI(alpha);
	DrawLockonLevelGUI(alpha);
	DrawLockonNumGUI(alpha);
	DrawScoreMagniGauge(alpha);
	NumGUI::GetInstance()->SetAlpha(1.0f);

	DrawPowerUpTelop();
	DrawBonusTelop();
	DrawBonusTimeGUI();
	DrawBonusPositionTelop();
	DrawCautionTelop();
}

/**************************************
ゲームオーバーシーン描画処理
***************************************/
void DrawGUIonGameoverScene(void)
{
	DrawGameoverTelop();
}

/**************************************
ステージクリアシーン描画処理
***************************************/
void DrawGUIonStageClearScene(void)
{
	DrawStageClearTelop();
}

/**************************************
ネームエントリーシーン描画処理
***************************************/
void DrawGUIonNameEntryScene(void)
{
	DrawNameEntryRanking();
	DrawRankingScore();
	DrawRankingName();
	DrawNameEntryCursor();
	DrawRankingTelop();
	DrawEntryTelop();
}

/**************************************
数字描画処理
***************************************/
void DrawGUINum(GUI_NUMTEXTURE textureID, int num, VERTEX_2D *vtxWk)
{
	int x = num % GUI_NUMTEX_DIVIDE_X;
	int y = num / GUI_NUMTEX_DIVIDE_X;
	float sizeX = 1.0f / GUI_NUMTEX_DIVIDE_X;
	float sizeY = 1.0f / GUI_NUMTEX_DIVIDE_Y;

	vtxWk[0].tex = D3DXVECTOR2(x * sizeX, y * sizeY);
	vtxWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, y * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(x * sizeX, (y + 1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, (y + 1) * sizeY);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, texture[textureID]);
	
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}
