//=====================================
//
//ゲームオーバーシーン処理[gameoverScene.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "gameoverScene.h"
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
#include "particleManager.h"
#include "sceneFade.h"


/**************************************
マクロ定義
***************************************/
#define GAMEOVER_DURATION		(300)

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
HRESULT InitGameoverScene(int num)
{
	//NOTE:バトルシーンの状態を使い回すので各オブジェクトの初期化はしない

	SetMonotoneEffect(true);
	SetGameoverTelop();
	cntFrame = 0;
	FadeInBGM(BGM_GAMEOVER, 30, false);
	return S_OK;
}

/**************************************
終了処理
***************************************/
void UninitGameoverScene(int num)
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
	//UninitParticleManager(num);

	SetMonotoneEffect(false);
	FadeOutBGM(BGM_GAMEOVER, 10);
}

/**************************************
更新処理
***************************************/
void UpdateGameoverScene(void)
{

	UpdatePlayerBullet();
	//UpdatePlayerModel();
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
	if (cntFrame == GAMEOVER_DURATION)
	{
		SetSceneFade(TitleScene);
	}
}

/**************************************
描画処理
***************************************/
void DrawGameoverScene(void)
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

	DrawRockonSite();
	DrawTargetSite();
}
