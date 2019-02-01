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
	BGM_TITLESCENE,
	BGM_TUTORIALSCENE,
	BGM_BONUSTIME,
	BGM_GAMEOVER,
	BGM_STAGECLEAR,
	BGM_NAMEENTRY,
	BGM_MAX
};

enum BGM_STATE
{
	BGM_NORMAL,
	BGM_FADEIN,
	BGM_FADEOUT,
	BGM_STATEMAX
};

typedef struct _BGM{
	LPDIRECTSOUNDBUFFER8 clip;
	int fadeDuration;
	int cntFrame;
	float volume;
	float targetVolume;
	int state;

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
void FadeInBGM(DEFINE_BGM bgm, int duration, bool isResume = false);
void FadeOutBGM(DEFINE_BGM bgm, int duration);

#endif