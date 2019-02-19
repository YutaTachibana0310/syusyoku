//=====================================
//
//stageClearScene処理[stageClearScene.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "stageClearScene.h"
#include "sceneManager.h"
#include "input.h"
#include "playerModel.h"
#include "battleCamera.h"
#include "playerBullet.h"
#include "targetSite.h"
#include "rockonSite.h"
#include "meshCylinder.h"
#include "playerMissile.h"
#include "playerMissileSmog.h"
#include "particleManager.h"
#include "enemyManager.h"
#include "playerBulletTrail.h"
#include "collisionManager.h"
#include "battleController.h"
#include "bgmManager.h"
#include "monotone.h"
#include "gameoverTelop.h"
#include "sceneFade.h"
#include "stageClearTelop.h"
#include "particleManager.h"

/**************************************
マクロ定義
***************************************/
#define STAGECLEAR_DURATION			(240)
			
/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static int cntFrame;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
HRESULT InitStageClearScene(int num)
{
	//NOTE*バトルシーンの状態を使い回すので各オブジェクトの初期化はしない

	//TODO:テロップ再生,BGM再生

	SetStageClearTelop();
	ChangeStatePlayerModel(PlayerTitleLaunch);
	cntFrame = 0;
	FadeInBGM(BGM_STAGECLEAR, 30);
	return S_OK;
}

/**************************************
終了処理
***************************************/
void UninitStageClearScene(int num)
{
	UninitPlayerModel(num);
	UninitPlayerBullet(num);
	UninitTargetSite(num);
	UninitRockonSite(num);
	UninitMeshCylinder(num);
	UninitPlayerMissile(num);
	//UninitPlayerMissileSmog(num);
	UninitPlayerBulletTrail(num);
	UninitEnemyManager(num);
}

/**************************************
更新処理
***************************************/
void UpdateStageClearScene(void)
{
	UpdatePlayerBullet();
	UpdatePlayerModel();
	UpdateBattleCamera();

	UpdateRockonSite();

	UpdateMeshCylinder();

	UpdatePlayerMissile();
	//UpdatePlayerMissileSmog();
	UpdatePlayerBulletTrail();

	UpdateParticleManager();

	UpdateEnemyManager();

	CheckEnemyCollision();
	UpdateCollisionManager();

	cntFrame++;
	if (cntFrame == STAGECLEAR_DURATION)
	{
		SetSceneFade(NameEntryScene);
		FadeOutBGM(BGM_STAGECLEAR, 60);
	}
}

/**************************************
描画処理
***************************************/
void DrawStageClearScene(void)
{
	SetBattleCamera();
	DrawMeshCylinder();

	DrawEnemyManager();

	DrawPlayerModel();

	DrawPlayerMissile();
	//DrawPlayerMissileSmog();
	DrawPlayerBullet();
	DrawPlayerBulletTrail();

	DrawParticleManager();
}
