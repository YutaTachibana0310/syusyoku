//=============================================================================
//
// バトルシーン処理 [battleScene.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "battleScene.h"
#include "input.h"
#include "cloud.h"
#include "enemyMissile.h"
#include "playerModel.h"
#include "battleCamera.h"
#include "playerBullet.h"
#include "targetSite.h"
#include "rockonSite.h"
#include "meshCylinder.h"
#include "scoreGUI.h"
#include "playerMissile.h"
#include "playerMissileSmog.h"
#include "particleManager.h"
#include "scoreGUI.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BATTLESCENE_TEXTURE_NAME	_T("data/TEXTURE/UI/battle.png")	// プレイヤーバレットのテクスチャ

#define BATTLESCENE_TEXTURE_SIZE_X (200)			// テクスチャサイズX
#define BATTLESCENE_TEXTURE_SIZE_Y (200)			// テクスチャサイズY

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBattleScene(int num)
{
	InitCloud(num);
	InitEnemyMissile(num);
	InitTargetSite(num);
	InitRockonSite(num);
	InitPlayerModel(num);
	InitPlayerBullet(num);
	InitBattleCamera();
	InitMeshCylinder(num);
	InitScoreGUI(num);
	InitPlayerMissile(num);
	InitPlayerMissileSmog(num);
	InitScoreGUI(num);

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBattleScene(int num)
{
	//UninitCloud(num);
	UninitEnemyMissile(num);
	UninitPlayerModel(num);
	UninitPlayerBullet(num);
	UninitTargetSite(num);
	UninitRockonSite(num);
	UninitMeshCylinder(num);
	UninitScoreGUI(num);
	UninitPlayerMissile(num);
	UninitPlayerMissileSmog(num);
	UninitScoreGUI(num);
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateBattleScene(void)
{
	UpdateCloud();
	UpdateEnemyMissile();
	UpdatePlayerBullet();
	UpdatePlayerModel();
	UpdateBattleCamera();
	UpdateTargetSite();
	UpdateRockonSite();
	UpdateMeshCylinder();
	UpdatePlayerMissile();
	UpdateScoreGUI();
	UpdatePlayerMissileSmog();
	UpdateScoreGUI();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetScene(ResultScene);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawBattleScene(void)
{
	//DrawScoreGUI();
	SetBattleCamera();
	DrawMeshCylinder();
	//DrawCloud();
	DrawEnemyMissile();
	DrawPlayerModel();
	DrawPlayerMissile();
	DrawPlayerMissileSmog();
	DrawPlayerBullet();
	DrawParticleManager();
	DrawRockonSite();
	DrawTargetSite();
	DrawScoreGUI();
}
