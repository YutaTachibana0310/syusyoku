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
#include "playerModel.h"
#include "battleCamera.h"
#include "playerBullet.h"
#include "targetSite.h"
#include "rockonSite.h"
#include "meshCylinder.h"
#include "playerMissile.h"
#include "playerMissileSmog.h"
#include "particleManager.h"
//#include "GUIManager.h"
#include "enemyManager.h"
#include "playerBulletTrail.h"
#include "collisionManager.h"
#include "battleController.h"
#include "bgmManager.h"
#include "dataContainer.h"
#include "stageData.h"
#include "hardCubeObject.h"

#include "debugWindow.h"
#include "DebugTimer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define BATTLESCENE_LABEL "BattleScene"

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void DrawDebugWindowBattleScene(void);

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
	if (num != 0)
	{
		//SetBackColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	InitDataContainer(num);
	InitCloud(num);
	InitTargetSite(num);
	InitRockonSite(num);
	InitPlayerModel(num);
	InitPlayerBullet(num);
	InitBattleCamera();
	InitMeshCylinder(num);
	InitPlayerMissile(num);
	//InitPlayerMissileSmog(num);
	InitPlayerBulletTrail(num);
	InitEnemyManager(num);
	InitBattleController(num);
	

	RegisterDebugTimer(BATTLESCENE_LABEL);
	PlayBGM(BGM_BATTLESCENE);

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBattleScene(int num)
{
	//NOTE:ゲームオーバーシーンで状態を使い回すのでマスク

	UninitBattleController(num);
	//UninitCloud(num);
	//UninitPlayerModel(num);
	//UninitPlayerBullet(num);
	//UninitTargetSite(num);
	//UninitRockonSite(num);
	//UninitMeshCylinder(num);
	//UninitPlayerMissile(num);
	//UninitPlayerMissileSmog(num);
	//UninitPlayerBulletTrail(num);
	//UninitEnemyManager(num);
	FadeOutBGM(BGM_BATTLESCENE, 30);
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateBattleScene(void)
{
	UpdateBattleController();

	//GetTimerCount(&startPlayerUpdate);
	CountDebugTimer(BATTLESCENE_LABEL, "PlayerUpdate");
	UpdatePlayerBullet();
	UpdatePlayerModel();
	UpdatePlayerBulletTrail();
	CountDebugTimer(BATTLESCENE_LABEL, "PlayerUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "SiteUpdate");
	UpdateRockonSite();

	CountDebugTimer(BATTLESCENE_LABEL, "SiteUpdate");

	UpdateMeshCylinder();

	CountDebugTimer(BATTLESCENE_LABEL, "PMissileUpdate");
	UpdatePlayerMissile();
	CountDebugTimer(BATTLESCENE_LABEL, "PMissileUpdate");

	//CountDebugTimer(BATTLESCENE_LABEL, "MissileSmogUpdate");
	////UpdatePlayerMissileSmog();
	//CountDebugTimer(BATTLESCENE_LABEL, "MissileSmogUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "ParticleUpdate");
	UpdateParticleManager();
	CountDebugTimer(BATTLESCENE_LABEL, "ParticleUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "EnemyUpdate");
	UpdateEnemyManager();
	CountDebugTimer(BATTLESCENE_LABEL, "EnemyUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "CollisionUpdate");
	CheckEnemyCollision();
	UpdateCollisionManager();
	CountDebugTimer(BATTLESCENE_LABEL, "CollisionUpdate");

	//ゲームオーバー判定
	if (GetPlayerHP() <= 0.0f)
	{
		SetScene(GamveoverScene);
	}

	//ゲームクリア判定
	if (IsStageEnd() && IsAllHardCubeDisable())
	{
		SetScene(StageClearScene);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawBattleScene(void)
{
	SetBattleCamera();
	DrawMeshCylinder();
	//DrawCloud();

	CountDebugTimer(BATTLESCENE_LABEL, "EnemyDraw");
	DrawEnemyManager();
	CountDebugTimer(BATTLESCENE_LABEL, "EnemyDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "PlayerDraw");
	DrawPlayerModel();
	CountDebugTimer(BATTLESCENE_LABEL, "PlayerDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "PMissileDraw");
	DrawPlayerMissile();
	//DrawPlayerMissileSmog();
	CountDebugTimer(BATTLESCENE_LABEL, "PMissileDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "ParticleDraw");
	DrawParticleManager();
	CountDebugTimer(BATTLESCENE_LABEL, "ParticleDraw");

	//パーティクルマネージャ内で描画
	//DrawPlayerBullet();
	//DrawPlayerBulletTrail();

	CountDebugTimer(BATTLESCENE_LABEL, "siteDraw");
	DrawRockonSite();
	DrawTargetSite();
	CountDebugTimer(BATTLESCENE_LABEL, "siteDraw");

	DrawDebugTimer("BattleScene");
}