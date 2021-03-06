//=====================================
//
//コリージョンマネージャヘッダ[collisionManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define COLLISION_DIVISIONSPACE_LEVEL_MAX		(6)			//空間レベルの最大レベル

/**************************************
構造体定義
***************************************/
struct _CCell;

//四分木分割空間に登録されるリストオブジェクト構造体
typedef struct _OBJECT_FOR_TREE {
	_CCell *registerSpace;		//登録空間
	void *object;				//対象オブジェクトへの汎用ポインタ
	_OBJECT_FOR_TREE *prev;		//前のリストオブジェクト
	_OBJECT_FOR_TREE *next;		//後のリストオブジェクト
#ifdef _DEBUG
	DWORD elem;
#endif
}OBJECT_FOR_TREE;

//分割空間構造体
typedef struct _CCell {
	OBJECT_FOR_TREE *latestObj;	//空間に登録されている先頭のオブジェクトへのポインタ
}CCell;

//空間に登録されるオブジェクトを識別するID(OFT : OBJECT_FOR_TREE)
enum OFT_ID
{
	OFT_PLAYERBULLET,
	OFT_CUBEOBJECT,
	OFT_HARDCUBE,
	OFT_BONUSCUBE,
	OFT_PLAYER,
	OFT_ENEMYBULLET,
	OFT_ENEMYHOMINGBULLET,
	OFT_ID_MAX
};

//コリージョンマネージャ構造体
typedef struct {
	unsigned int spaceNum[COLLISION_DIVISIONSPACE_LEVEL_MAX];	//各レベルの空間数
	DWORD cellNum;												//空間数の総和
	CCell **cellArray[OFT_ID_MAX];								//オブジェクトが登録される空間配列
	bool useDivSpace;											//空間分割を使用するかどうか

}COLLISION_MANAGER;
/**************************************
プロトタイプ宣言
***************************************/
void InitCollisionManager(int num);
void UninitCollisionManager(int num);
void UpdateCollisionManager(void);
void SetUseDivisionSpace(bool state);		//空間分割の使用切り替え処理
//void SetUseDivideSpace(bool state);			

//空間にオブジェクトを登録する処理
//bool RegisterObjectToSpace(float left, float top, float right, float bottom, OBJECT_FOR_TREE *obj, OFT_ID id);
bool RegisterObjectToSpace(COLLIDER_CUBE *collider, OBJECT_FOR_TREE *obj, OFT_ID id);
//空間からオブジェクトを取り除く処理
bool RemoveObjectFromSpace(OBJECT_FOR_TREE *obj);
//OBJECT_FOT_TREE作成処理
void CreateOFT(OBJECT_FOR_TREE *oft, void *object);
#endif