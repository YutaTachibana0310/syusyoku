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
	if (num != 0)
	{
		SetBackColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

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
	UpdateCloud();
	UpdatePlayerBullet();
	UpdatePlayerModel();
	UpdateBattleCamera();
	UpdateTargetSite();
	UpdateRockonSite();
	UpdateMeshCylinder();
	UpdatePlayerMissile();
	UpdatePlayerMissileSmog();
	UpdatePlayerBulletTrail();
	UpdateGUIManager();
	UpdateEnemyManager();

	CheckEnemyCollision();

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
	DrawEnemyManager();
	DrawPlayerModel();
	DrawPlayerMissile();
	DrawPlayerMissileSmog();
	DrawPlayerBullet();
	DrawPlayerBulletTrail();
	DrawParticleManager();
	DrawRockonSite();
	DrawTargetSite();
	DrawGUIManager();
}