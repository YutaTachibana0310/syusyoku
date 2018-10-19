//===================================
//
//エネミーミサイル発射ヘッダ[enemyMissileLaunch.h]
//Author:GP11A341 21 立花雄太
//
//===================================
#ifndef _ENEMYMISSILELAUNCH_H_
#define _ENEMYMISSILELAUNCH_H_

#include "enemyMissile.h"

/************************************
マクロ定義
*************************************/

/************************************
構造体定義
*************************************/

/************************************
プロトタイプ宣言
*************************************/
void EnemyMissileLaunchEnter(ENEMYMISSILE *ptr);
void EnemyMissileLaunchUpdate(ENEMYMISSILE *ptr);
void EnemyMissileLaunchExit(ENEMYMISSILE *ptr);

#endif
