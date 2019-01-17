//=====================================
//
//BGMマネージャヘッダ[bgmManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BGMMANAGER_H_
#define _BGMMANAGER_H_

#include "main.h"

#include <dsound.h>
#include <mmsystem.h>

/**************************************
マクロ定義
***************************************/
#define BGM_VOLUME_MAX		(100.0f)
#define BGM_VOLUME_MIN		(0.0f)

/**************************************
構造体定義
***************************************/
enum DEFINE_BGM
{
	BGM_BATTLESCENE,
	BGM_MAX
};

typedef struct {
	LPDIRECTSOUNDBUFFER8 clip;

}BGM;
/**************************************
プロトタイプ宣言
***************************************/
void InitBgmManager(int num);
void UninitBgmManager(int num);
void UpdateBgmManager(void);

void PlayBGM(DEFINE_BGM bgm);
void StopBGM(DEFINE_BGM bgm);
void ResumeBGM(DEFINE_BGM bgm);
bool IsPlayingBGM(DEFINE_BGM bgm);

#endif