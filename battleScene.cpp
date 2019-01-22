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
#include "GUIManager.h"
#include "enemyManager.h"
#include "playerBulletTrail.h"
#include "collisionManager.h"
#include "battleController.h"
#include "bgmManager.h"

#include "debugWindow.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

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
static LARGE_INTEGER startPlayerUpdate, endPlayerUpdate;
static LARGE_INTEGER startPMissileUpdate, endPMissileUpdate;
static LARGE_INTEGER startEnemyUpdate, endEnemyUpdate;
static LARGE_INTEGER startSiteUpdate, endSiteUpdate;
static LARGE_INTEGER startPlayerDrawm, endPlayerDraw;
static LARGE_INTEGER startPMissileDraw, endPMissileDraw;
static LARGE_INTEGER startEnemyDraw, endEnemyDraw;
static LARGE_INTEGER startSiteDraw, endSiteDraw;
static LARGE_INTEGER startParticleUpdate, endParticleUpdate;
static LARGE_INTEGER startParticleDraw, endParticleDraw;
static LARGE_INTEGER startCollision, endCollision;

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitBattleScene(int num)
{
	if (num != 0)
	{
		//SetBackColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		PlayBGM(BGM_BATTLESCENE);
	}

	InitBattleController(num);

	InitGUIManager(num);
	InitCloud(num);
	InitTargetSite(num);
	InitRockonSite(num);
	InitPlayerModel(num);
	InitPlayerBullet(num);
	InitBattleCamera();
	InitMeshCylinder(num);
	InitPlayerMissile(num);
	InitPlayerMissileSmog(num);
	InitPlayerBulletTrail(num);
	InitEnemyManager(num);


	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitBattleScene(int num)
{
	UninitBattleController(num);

	UninitCloud(num);
	UninitPlayerModel(num);
	UninitPlayerBullet(num);
	UninitTargetSite(num);
	UninitRockonSite(num);
	UninitMeshCylinder(num);
	UninitPlayerMissile(num);
	UninitPlayerMissileSmog(num);
	UninitPlayerBulletTrail(num);
	UninitGUIManager(num);
	UninitEnemyManager(num);
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateBattleScene(void)
{
	UpdateBattleController();

	GetTimerCount(&startPlayerUpdate);
	UpdatePlayerBullet();
	UpdatePlayerModel();
	UpdateBattleCamera();
	GetTimerCount(&endPlayerUpdate);

	GetTimerCount(&startSiteUpdate);
	UpdateRockonSite();
	GetTimerCount(&endSiteUpdate);

	UpdateMeshCylinder();

	GetTimerCount(&startPMissileUpdate);
	UpdatePlayerMissile();
	UpdatePlayerMissileSmog();
	UpdatePlayerBulletTrail();
	GetTimerCount(&endPMissileUpdate);

	GetTimerCount(&startParticleUpdate);
	UpdateParticleManager();
	GetTimerCount(&endParticleUpdate);

	UpdateGUIManager();

	GetTimerCount(&startEnemyUpdate);
	UpdateEnemyManager();
	GetTimerCount(&endEnemyUpdate);

	GetTimerCount(&startCollision);
	CheckEnemyCollision();
	UpdateCollisionManager();
	GetTimerCount(&endCollision);

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
	SetBattleCamera();
	DrawMeshCylinder();
	//DrawCloud();

	GetTimerCount(&startEnemyDraw);
	DrawEnemyManager();
	GetTimerCount(&endEnemyDraw);

	GetTimerCount(&startPlayerDrawm);
	DrawPlayerModel();
	GetTimerCount(&endPlayerDraw);

	GetTimerCount(&startPMissileDraw);
	DrawPlayerMissile();
	DrawPlayerMissileSmog();
	DrawPlayerBullet();
	DrawPlayerBulletTrail();
	GetTimerCount(&endPMissileDraw);

	GetTimerCount(&startParticleDraw);
	DrawParticleManager();
	GetTimerCount(&endParticleDraw);

	GetTimerCount(&startSiteDraw);
	DrawRockonSite();
	DrawTargetSite();
	GetTimerCount(&endSiteDraw);

	DrawGUIManager();

	DrawDebugWindowBattleScene();
}

/******************************************************************************
デバッグウィンドウ表示
******************************************************************************/
void DrawDebugWindowBattleScene(void)
{
	BeginDebugWindow("BattleScene");

	DebugTreeExpansion(true);
	if (DebugTreePush("Player"))
	{
		DebugText("PlayerUpdate   : %fmsec", CalcProgressTime(startPlayerUpdate, endPlayerUpdate));
		DebugText("PlayerDraw     : %fmsec", CalcProgressTime(startPlayerDrawm, endPlayerDraw));
		DebugTreePop();
	}

	DebugTreeExpansion(true);
	if (DebugTreePush("PlayerMissile"))
	{
		DebugText("PMissileUpdate : %fmsec", CalcProgressTime(startPMissileUpdate, endPMissileUpdate));
		DebugText("PMissileDraw   : %fmsec", CalcProgressTime(startPMissileDraw, endPMissileDraw));
		DebugTreePop();
	}

	DebugTreeExpansion(true);
	if (DebugTreePush("Enemy"))
	{
		DebugText("EnemyUpdate    : %fmsec", CalcProgressTime(startEnemyUpdate, endEnemyUpdate));
		DebugText("EnemyDraw      : %fmsec", CalcProgressTime(startEnemyDraw, endEnemyDraw));
		DebugTreePop();
	}

	DebugTreeExpansion(true);
	if (DebugTreePush("Particle"))
	{
		DebugText("ParticleUpdate : %fmsec", CalcProgressTime(startParticleUpdate, endParticleUpdate));
		DebugText("ParticleDraw   : %fmsec", CalcProgressTime(startParticleDraw, endParticleDraw));
		DebugTreePop();
	}

	DebugTreeExpansion(true);
	if (DebugTreePush("Site"))
	{
		DebugText("SiteUpdate     : %fmsec", CalcProgressTime(startSiteUpdate, endSiteUpdate));
		DebugText("SiteDraw       : %fmsec", CalcProgressTime(startSiteDraw, endSiteDraw));
		DebugTreePop();
	}

	DebugTreeExpansion(true);
	if (DebugTreePush("Collision"))
	{
		DebugText("Collision		: %fmsec", CalcProgressTime(startCollision, endCollision));
		DebugTreePop();
	}

	EndDebugWindow("BattleScene");
}