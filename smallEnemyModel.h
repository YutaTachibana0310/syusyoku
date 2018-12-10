//=====================================
//
//�e���v���[�g�w�b�_[template.h]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#ifndef _SMALLENEMYMODEL_H_
#define _SMALLENEMYMODEL_H_

#include "main.h"

/**************************************
�}�N����`
***************************************/
#define SMALLENEMY_MAX		(128)

/**************************************
�\���̒�`
***************************************/
enum SMALLENEMY_STATE
{
	SmallEnemyMove,
	SmallEnemyAttack,
	SmallEnemyEscape,
	SmallEnemyStateMax
};

typedef struct
{
	bool active;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int cntFrame;
	int state;

	D3DXVECTOR3 startPos, goalPos;
	D3DXVECTOR3 startRot, goalRot;

	float hp;
	int atkNum;

}SMALLENEMYMODEL;

/**************************************
�v���g�^�C�v�錾
***************************************/
void InitSmallEnemyModel(int num);
void UninitSmallEnemyModel(int num);
void UpdateSmallEnemyModel(void);
void DrawSmallEnemyModel(void);
void SetSmallEnemyModel(D3DXVECTOR3 pos);

//�X�V�����e�[�u��
void UpdateSmallEnemyMove(SMALLENEMYMODEL *enemy);
void UpdateSmallEnemyAttack(SMALLENEMYMODEL *enemy);
void UpdateSmallEnemyEscape(SMALLENEMYMODEL *enemy);

//���ꏈ���e�[�u��
void EnterSmallEnemyMove(SMALLENEMYMODEL *enemy);
void EnterSmallEnemyAttack(SMALLENEMYMODEL *enemy);
void EnterSmallEnemyEscape(SMALLENEMYMODEL *enemy);

//�ޏꏈ���e�[�u��
void ExitSmallEnemyMove(SMALLENEMYMODEL *enemy);
void ExitSmallEnemyAttack(SMALLENEMYMODEL *enemy);
void ExitSmallEnemyEscape(SMALLENEMYMODEL *enemy);

#endif