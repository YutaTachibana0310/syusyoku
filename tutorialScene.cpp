//=====================================
//
//チュートリアルシーン処理[tutorialScene.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "tutorialScene.h"
#include "playerModel.h"
#include "battleCamera.h"
#include "playerBullet.h"
#include "stageData.h"
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
#include "tutorialController.h"
#include "bgmManager.h"
#include "input.h"
#include "sceneFade.h"
#include "soundEffectManager.h"

/**************************************
マクロ定義
***************************************/
#define TUTORIALSCENE_FADE_DURATION		(60)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
HRESULT InitTutorialScene(int num)
{
	InitGUIManager(num);
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
	InitTutorialController(num);

	FadeInBGM(BGM_TUTORIALSCENE, TUTORIALSCENE_FADE_DURATION);
	return S_OK;
}

/**************************************
終了処理
***************************************/
void UninitTutorialScene(int num)
{
	UninitGUIManager(num);
	UninitTargetSite(num);
	UninitRockonSite(num);
	UninitPlayerModel(num);
	UninitPlayerBullet(num);
	UninitMeshCylinder(num);
	UninitPlayerMissile(num);
	UninitPlayerMissileSmog(num);
	UninitPlayerBulletTrail(num);
	UninitEnemyManager(num);
	UninitTutorialController(num);
}

/**************************************
更新処理
***************************************/
void UpdateTutorialScene(void)
{
	UpdateBattleCamera();

	UpdatePlayerModel();
	UpdatePlayerBullet();

	UpdateRockonSite();

	UpdateMeshCylinder();

	UpdatePlayerMissile();
	UpdatePlayerMissileSmog();
	UpdatePlayerBulletTrail();

	UpdateParticleManager();

	UpdateGUIManager();

	UpdateEnemyManager();

	CheckEnemyCollision();
	UpdateCollisionManager();

	UpdateTutorialController();

	if (GetKeyboardPress(DIK_RETURN))
	{
		FadeOutBGM(BGM_TUTORIALSCENE, TUTORIALSCENE_FADE_DURATION);
		SetSceneFade(BattleScene);
	}
}

/**************************************
描画処理
***************************************/
void DrawTutorialScene(void)
{
	SetBattleCamera();

	DrawMeshCylinder();

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

	DrawTutorialController();
}
