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

using namespace std;

/**************************************
マクロ定義
***************************************/
#define SOUND_VOLUME_MAX	(100.0f)		//最大音量
#define SOUND_VOLUME_MIN	(0.0f)			//最小音量

/**************************************
構造体定義
***************************************/
enum class  DefineSE : unsigned int
{
	//サウンド通しナンバー
	LOCKON,
	MISSILELAUNCH,
	SMALLEXPL,
	READY,
	DECISION,
	SHOT,
	MIDDLEEXPL,
	BONUSEXPL,
	BONUSSTART,
	NORMALEXPL,
	CURSOR,
	POWERUP,
	HOMINGBULLET,
	ENEMYBULLET,
	MENUDICISION,
	PAUSE,
	CAUTION,
	NOISE,
	MAX
};

typedef struct
{
	LPDIRECTSOUNDBUFFER8 clip;
	bool playOrder;
	float volume;
}SOUNDEFFECT;

/**************************************
プロトタイプ宣言
***************************************/
void InitSoundEffectManager(int num);
void UninitSoundEffectManager(int num);
void UpdateSoundEffectManager(void);

void PlaySE(DefineSE sound);
void PlaySE_3D(DefineSE sound, float posZ);
void StopSE(DefineSE sound);
void ResumeSE(DefineSE sound);
bool IsPlayingSE(DefineSE sound);

#endif