//=====================================
//
//BGMマネージャ処理[bgmManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bgmManager.h"
#include "sound.h"
#include <tchar.h>

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static const TCHAR* BgmFileName[BGM_MAX] = {
	_T("data/SOUND/LAST_GUARDIAN.wav"),
};

static BGM bgmData[BGM_MAX];

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitBgmManager(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		BGM *ptr = &bgmData[0];
		for (int i = 0; i < BGM_MAX; i++, ptr++)
		{
			ptr->clip = LoadSound(&BgmFileName[i][0]);
			SetSoundVolume(ptr->clip, SOUND_VOLUME_INIT);
		}
		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitBgmManager(int num)
{
	if (num == 0)
	{
		BGM *ptr = &bgmData[0];
		for (int i = 0; i < BGM_MAX; i++, ptr++)
		{
			SAFE_RELEASE(ptr->clip);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateBgmManager(void)
{

}

/**************************************
再生処理
***************************************/
void PlayBGM(DEFINE_BGM bgm)
{
	PlaySoundBuffer(bgmData[bgm].clip, E_DS8_FLAG_LOOP, true);
}

/**************************************
停止
***************************************/
void StopBGM(DEFINE_BGM bgm)
{
	StopSoundBuffer(bgmData[bgm].clip);
}

/**************************************
再開
***************************************/
void ResumeBGM(DEFINE_BGM bgm)
{
	PlaySoundBuffer(bgmData[bgm].clip, E_DS8_FLAG_LOOP, false);
}

/**************************************
再生中判定
***************************************/
bool IsPlayingBGM(DEFINE_BGM bgm)
{
	return IsPlaying(bgmData[bgm].clip);
}