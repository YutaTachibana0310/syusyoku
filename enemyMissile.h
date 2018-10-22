//============================================
//
//�G�l�~�[�~�T�C���w�b�_[enemyMissile.h]
//AUthor:GP11A341 21 ���ԗY��
//
//============================================
#ifndef _ENEMYMISSILE_H_
#define _ENEMYMISSILE_H_

#include "main.h"
#include "collider.h"

/**********************************************
�}�N����`
**********************************************/

/**********************************************
�\���̒�`
**********************************************/
typedef struct
{
	bool active;					//�g�p�t���O
	D3DXVECTOR3 pos;				//���W
	D3DXQUATERNION rot;				//��]�N�H�[�^�j�I��
	D3DXVECTOR3 moveDir;			//�ړ�����
	D3DXVECTOR3 targetPos;			//�ڕW���W
	SPHERE collider;				//�����蔻��

	float speed;					//�ړ��X�s�[�h
	bool flgHoming;					//�z�[�~���O�t���O
	int cntFrame;					//�t���[���J�E���g

	int state;						//���
	int nextState;					//���̏��
}ENEMYMISSILE;

/********************************************
�񋓑̒�`
*********************************************/
enum ENEMYMISSILE_STATE
{
	ENEMYMISSILE_LAUNCH,			//���˒�����
	ENEMYMISSILE_HOMING,			//�z�[�~���O���
	ENEMYMISSILE_STRAIGHT,			//���i���
	ENEMYMISSILE_STATEMAX
};

/**********************************************
�v���g�^�C�v�錾
**********************************************/
void InitEnemyMissile(int num);
void UninitEnemyMissile(void);
void UpdateEnemyMissile(void);
void DrawEnemyMissile(void);
ENEMYMISSILE *GetEnemyMissileAdr(int n);
void ChangeStateEnemyMissile(ENEMYMISSILE *ptr, int targetState);
#endif