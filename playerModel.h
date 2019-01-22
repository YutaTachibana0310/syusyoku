//=====================================
//
//プレイヤーモデルヘッダ[playerModel.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERMODEL_H_
#define _PLAYERMODEL_H_

#include "main.h"
#include "EasingVector.h"
#include "battleCamera.h"
#include "rockonSite.h"

#define PLAYER_INITMOVE_FRAME		(30)
#define PLAYERMODEL_MAX				(1)
#define PLAYER_DESTROT_MAX			(0.51f)
#define PLAYER_MOVERANGE			(50.0f)
#define PLAYER_ROCKON_MAX			(80)
#define PLAYER_HOMINGATK_INTERBAL	(60)
#define PLAYER_SHOT_INTERBAL		(5)
/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
//プレイヤーステート
enum PlayerState
{
	PlayerFPS,
	PlayerTopView,
	PlayerSideView,
	PlayerQuaterView,
	PlayerTransition,
	PlayerTitle,
	PlayerTitleLaunch,
	PlayerTutorialMove,
	PlayerTutorialLockon,
	PlayerStateMax,
};

//ロックオン対象構造体
typedef struct
{
	bool use;			//使用フラグ
	D3DXVECTOR3 *pos;	//ターゲット座標
	bool *active;		//ターゲット状態
	float *hp;			//ターゲットHP
}ROCKONTARGET;

//プレイヤー構造体
typedef struct
{

	bool active;
	int id;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;
	D3DXMATRIX mtxWorld;

	D3DXVECTOR3 initPos;
	int cntFrame;
	D3DXVECTOR3 destRot;
	int nextState;

	//ロックオン関連パラメータ
	ROCKONTARGET target[PLAYER_ROCKON_MAX];
	int atkInterbal;
	int lockonNum;
	bool boostMode;

	D3DXVECTOR3 shotpos1, shotpos2;

}PLAYERMODEL;

/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerModel(int num);
void UninitPlayerModel(int num);
void UpdatePlayerModel(void);
void DrawPlayerModel(void);
PLAYERMODEL *GetPlayerAdr(int num);
void ChangeStatePlayerModel(int next);
ROCKONTARGET *AddRockonTarget(int id, D3DXVECTOR3 *targetPos, bool *targetActive, float *targetHP);
void ReleaseRockonTarget(PLAYERMODEL *player, int targetID);
void AttackPlayerMissile(PLAYERMODEL *player);

//入場処理
void EnterPlayerModelTutorialMove(PLAYERMODEL *ptr);
void EnterPlayerModelTutorialLockon(PLAYERMODEL *ptr);

//更新処理
void UpdatePlayerModelTutorialMove(PLAYERMODEL *ptr);
void UpdatePlayerModelTutorialLockon(PLAYERMODEL *ptr);

//退場処理
void ExitPlayerModelTutorialMove(PLAYERMODEL *ptr);
void ExitPlayerModelTutorialLockon(PLAYERMODEL *ptr);

//チュートリアルロックオン回数取得処理
int GetTutorialLockOnNum(void);

//チュートリアルホーミング攻撃回数取得処理
int GetTutorialHomingAttackNum(void);

#endif