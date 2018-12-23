//=====================================
//
//サウンドエフェクトマネージャヘッダ[soundEffectManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SOUNDEFFECTMANAGER_H_
#define _SOUNDEFFECTMANAGER_H_

#include "main.h"

#include <dsound.h>
#include <mmsystem.h>

/**************************************
マクロ定義
***************************************/
#define SOUND_VOLUME_MAX	(100.0f)		//最大音量
#define SOUND_VOLUME_MIN	(0.0f)			//最小音量

/**************************************
構造体定義
***************************************/
enum DEFINE_SOUNDEFFECT
{
	//サウンド通しナンバー
	SOUND_LOCKON,
	//SOUND_S_EXPLOSION,
	SOUND_MAX
};

typedef struct
{
	LPDIRECTSOUNDBUFFER8 clip;
	bool playOrder;
}SOUNDEFFECT;

/**************************************
プロトタイプ宣言
***************************************/
void InitSoundEffectManager(int num);
void UninitSoundEffectManager(int num);
void UpdateSoundEffectManager(void);

void PlaySE(DEFINE_SOUNDEFFECT sound);
void StopSE(DEFINE_SOUNDEFFECT sound);
void ResumeSE(DEFINE_SOUNDEFFECT sound);
bool IsPlayingSE(DEFINE_SOUNDEFFECT sound);

#endif