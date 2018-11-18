//=====================================
//
//�e���v���[�g�w�b�_[playerBullet.h]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_

#include "main.h"

/**************************************
�}�N����`
***************************************/
#define PLAYERBULLET_MAX		(128)

/**************************************
�\���̒�`
***************************************/
typedef struct
{
	bool active;

	D3DXVECTOR3 pos, rot, scale;
	D3DXVECTOR3 moveDir;
	float rotation;
	float speed;

}PLAYERBULLET;
/**************************************
�v���g�^�C�v�錾
***************************************/
void InitPlayerBullet(int num);
void UninitPlayerBullet(int num);
void UpdatePlayerBullet(void);
void DrawPlayerBullet(void);
void SetPlayerBullet(D3DXVECTOR3 pos, float speed);
PLAYERBULLET *GetPlayerBulletAdr(int id);
#endif