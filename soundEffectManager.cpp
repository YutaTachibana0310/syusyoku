//=====================================
//
//サウンドエフェクトマネージャ処理[soundEffectManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "soundEffectManager.h"
#include "sound.h"
#include <tchar.h>

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
const TCHAR* soundFileName[SOUND_MAX] =
{
	_T("data/SOUND/hoge.wav"),
	_T("data/SOUND/hoge.wav"),
};

static IDirectSound8 *directSound = NULL;
static LPDIRECTSOUNDBUFFER8 soundBuffer[SOUND_MAX];

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitSoundEffectManager(int num)
{
	if (num == 0)
	{
		for (int i = 0; i < SOUND_MAX; i++)
		{
			soundBuffer[i] = LoadSound(&soundFileName[i][0]);
		}
	}

	return;
}

/**************************************
終了処理
***************************************/
void UninitSoundEffectManager(int num)
{
	if (num == 0)
	{
		for (int i = 0; i < SOUND_MAX; i++)
		{
			soundBuffer[i]->Release();
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateSoundEffectManager(void)
{

}

/**************************************
再生処理
***************************************/
void PlaySE(DEFINE_SOUNDEFFECT sound)
{
	PlaySoundBuffer(soundBuffer[sound], E_DS8_FLAG_NONE, true);
}

/**************************************
停止処理
***************************************/
void StopSE(DEFINE_SOUNDEFFECT sound)
{
	StopSoundBuffer(soundBuffer[sound]);
}

/**************************************
再開処理
***************************************/
void ResumeSE(DEFINE_SOUNDEFFECT sound)
{
	PlaySoundBuffer(soundBuffer[sound], E_DS8_FLAG_NONE, false);
}

/**************************************
再生中判定
***************************************/
bool IsPlayingSE(DEFINE_SOUNDEFFECT sound)
{
	return IsPlaying(soundBuffer[sound]);
}

/**************************************
音量設定
***************************************/
void SetSEVolume(DEFINE_SOUNDEFFECT sound, float volume)
{
	SetSoundVolume(soundBuffer[sound], volume);
}

