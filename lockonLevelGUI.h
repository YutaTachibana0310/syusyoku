//=====================================
//
//���b�N�I�����x��GUI�w�b�_[lockonLevelGUI.h]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#ifndef _LOCKONLEVELGUI_H_
#define _LOCKONLEVELGUI_H_

#include "main.h"
#include <stdio.h>
/**************************************
�}�N����`
***************************************/

/**************************************
�\���̒�`
***************************************/

/**************************************
�v���g�^�C�v�錾
***************************************/
void InitLockonLevelGUI(int num);
void UninitLockonLevelGUI(int num);
void UpdateLockonLevelGUI(void);
void DrawLockonLevelGUI(void);
void DrawLockOnlevelGUIDebug(void);
void SaveSettingLockonNumGUI(FILE *fp);
void LoadSettingsLockonLevelGUI(FILE *fp);
#endif