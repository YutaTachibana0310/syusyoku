//=============================================================================
//
// �v���C���[�o���b�g�w�b�_ [ScoreGUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SCOREGUI_H_
#define _SCOREGUI_H_

#include "main.h"

// �}�N����`


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x
}SCOREGUI;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScoreGUI(int num);							//�v���C���[�o���b�g����������
void UninitScoreGUI(int num);								//�v���C���[�o���b�g�I������
void UpdateScoreGUI(void);								//�v���C���[�o���b�g�X�V����
void DrawScoreGUI(void);								//�v���C���[�o���b�g�`�揈��

#endif