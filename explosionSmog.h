//=====================================
//
//�G�N�X�v���[�W�����X���b�O�w�b�_[explosionSmog.h]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#ifndef _EXPLOSIONSMOG_H_
#define _EXPLOSIONSMOG_H_

#include "main.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�\���̒�`
***************************************/

/**************************************
�v���g�^�C�v�錾
***************************************/
void InitExplosionSmog(int num);
void UninitExplosionSmog(int num);
void UpdateExplosionSmog(void);
void DrawExplosionSmog(LPDIRECT3DVERTEXDECLARATION9 declare, LPD3DXEFFECT effect);
void SetExplosionSmog(const D3DXVECTOR3 *pos);

#endif