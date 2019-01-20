//=====================================
//
//ハードキューブオブジェクトヘッダ[hardCubeObject.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _HARDCUBEOBJECT_H_
#define _HARDCUBEOBJECT_H_

#include "main.h"
#include "collider.h"
#include "stageData.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
	bool active;				//アクティブ判定
	WORD id;					//固有ID
	float moveSpeed;			//移動スピード
	D3DXVECTOR3 moveDir;		//移動方向
	D3DXVECTOR3 rotValue;		//回転量

	COLLIDER_CUBE collider;		//当たり判定
	float hp;					//HP

	D3DXVECTOR3 pos;			//座標
	D3DXVECTOR3 rot;			//回転量
	float scale;				//スケール

	int cntFrame;				//フレームカウント
	int currentState;			//状態

	D3DXVECTOR3 startPos;
	D3DXVECTOR3 goalPos;
	int moveDurtaion;

	int type;

	D3DXVECTOR3 controller1;
	D3DXVECTOR3 controller2;

}HARD_CUBE_OBJECT;

enum HARDCUBE_STATE
{
	HardCubeInit,
	HardCubeMove,
	HardCubeNormalAttack,
	HardCubeHomingAttack,
	HardCubeCharge,
	HardCubeEscape,
	HardCubeBezier,
	HardCubeStateMax
};

enum HARDCUBE_TYPE
{
	HardCubeNormalType,
	HardCubeHomingType,
	HardCubeChargeType,
	HardCubeBezierType,
	HardCubeTypeMax
};

/**************************************
プロトタイプ宣言
***************************************/
void InitHardCubeObject(int num);
void UninitHardCubeObject(int num);
void UpdateHardCubeObject(void);
void DrawHardCubeObject(void);

void LockonHardCubeObject(void);
bool SetHardCubeObject(D3DXVECTOR3 *setPos);
void DisableHardCube(HARD_CUBE_OBJECT *ptr);	//非アクティブ処理
void ChangeStateHardCube(HARD_CUBE_OBJECT *ptr, int nextState);
bool SetHardCubeObjectFromData(STAGE_DATA *data);

//各状態入場処理関数
void OnEnterHardCubeInit(HARD_CUBE_OBJECT *ptr);
void OnEnterHardCubeMove(HARD_CUBE_OBJECT *ptr);
void OnEnterHardCubeNormalAttack(HARD_CUBE_OBJECT *ptr);
void OnEnterHardCubeHomingAttack(HARD_CUBE_OBJECT *ptr);
void OnEnterHardCubeEscape(HARD_CUBE_OBJECT *ptr);
void OnEnterHardCubeCharge(HARD_CUBE_OBJECT *ptr);
void OnEnterHardCubeBezier(HARD_CUBE_OBJECT *ptr);

//各状態更新処理関数
void OnUpdateHardCubeInit(HARD_CUBE_OBJECT *ptr);
void OnUpdateHardCubeMove(HARD_CUBE_OBJECT *ptr);
void OnUpdateHardCubeNormalAttack(HARD_CUBE_OBJECT *ptr);
void OnUpdateHardCubeHomingAttack(HARD_CUBE_OBJECT *ptr);
void OnUpdateHardCubeEscape(HARD_CUBE_OBJECT *ptr);
void OnUpdateHardCubeCharge(HARD_CUBE_OBJECT *ptr);
void OnUpdateHardCubeBezier(HARD_CUBE_OBJECT *ptr);

#endif