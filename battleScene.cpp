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

	RegisterDebugTimer(BATTLESCENE_LABEL);

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

	//GetTimerCount(&startPlayerUpdate);
	CountDebugTimer(BATTLESCENE_LABEL, "PlayerUpdate");
	UpdatePlayerBullet();
	UpdatePlayerModel();
	UpdateBattleCamera();
	GetTimerCount(&endPlayerUpdate);
	CountDebugTimer(BATTLESCENE_LABEL, "PlayerUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "SiteUpdate");
	UpdateRockonSite();
	GetTimerCount(&endSiteUpdate);
	CountDebugTimer(BATTLESCENE_LABEL, "SiteUpdate");

	UpdateMeshCylinder();

	CountDebugTimer(BATTLESCENE_LABEL, "PMissileUpdate");
	UpdatePlayerMissile();
	UpdatePlayerMissileSmog();
	UpdatePlayerBulletTrail();
	CountDebugTimer(BATTLESCENE_LABEL, "PMissileUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "ParticleUpdate");
	UpdateParticleManager();
	CountDebugTimer(BATTLESCENE_LABEL, "ParticleUpdate");

	UpdateGUIManager();

	CountDebugTimer(BATTLESCENE_LABEL, "EnemyUpdate");
	UpdateEnemyManager();
	CountDebugTimer(BATTLESCENE_LABEL, "EnemyUpdate");

	CountDebugTimer(BATTLESCENE_LABEL, "CollisionUpdate");
	CheckEnemyCollision();
	UpdateCollisionManager();
	CountDebugTimer(BATTLESCENE_LABEL, "CollisionUpdate");

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

	CountDebugTimer(BATTLESCENE_LABEL, "EnemyDraw");
	DrawEnemyManager();
	CountDebugTimer(BATTLESCENE_LABEL, "EnemyDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "PlayerDraw");
	DrawPlayerModel();
	CountDebugTimer(BATTLESCENE_LABEL, "PlayerDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "PMissileDraw");
	DrawPlayerMissile();
	DrawPlayerMissileSmog();
	DrawPlayerBullet();
	DrawPlayerBulletTrail();
	CountDebugTimer(BATTLESCENE_LABEL, "PMissileDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "ParticleDraw");
	DrawParticleManager();
	CountDebugTimer(BATTLESCENE_LABEL, "ParticleDraw");

	CountDebugTimer(BATTLESCENE_LABEL, "siteDraw");
	DrawRockonSite();
	DrawTargetSite();
	CountDebugTimer(BATTLESCENE_LABEL, "siteDraw");

	DrawGUIManager();

	DrawDebugTimer("BattleScene");
}