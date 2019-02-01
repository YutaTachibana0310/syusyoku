//=====================================
//
//BGMマネージャ処理[bgmManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bgmManager.h"
#include "sound.h"
#include <tchar.h>
#include "Easing.h"
#include "debugWindow.h"
#include <stdio.h>

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
	_T("data/SOUND/Sword_dance.wav"),
	_T("data/SOUND/P.L.A.Y..wav"),
	_T("data/SOUND/Landing+On+The+Moon_jingle.wav"),
	_T("data/SOUND/gameover2.wav"),
	_T("data/SOUND/To_the_Next_Generation.wav"),
	_T("data/SOUND/Traffic_Jam.wav"),
};

static BGM bgmData[BGM_MAX];

/**************************************
プロトタイプ宣言
***************************************/
bool SaveSettingsBGM(void);
bool LoadSettingsBGM(void);
void DrawDebugWindowBGM(void);

/**************************************
初期化処理
***************************************/
void InitBgmManager(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		bool res = LoadSettingsBGM();

		BGM *ptr = &bgmData[0];
		for (int i = 0; i < BGM_MAX; i++, ptr++)
		{
			ptr->clip = LoadSound(&BgmFileName[i][0]);
			SetSoundVolume(ptr->clip, ptr->volume);
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
	BGM *ptr = &bgmData[0];
	for (int i = 0; i < BGM_MAX; i++, ptr++)
	{
		if (ptr->state == BGM_NORMAL)
			continue;

		ptr->cntFrame++;

		//フェード処理
		float t = (float)ptr->cntFrame / (float)ptr->fadeDuration;
		float setVolume = (ptr->state == BGM_FADEIN) ? EaseLinear(t, 0.0f, ptr->volume) : EaseLinear(t, ptr->volume, 0.0f);
		SetSoundVolume(ptr->clip, setVolume);

		if (ptr->cntFrame == ptr->fadeDuration)
		{
			if (ptr->state == BGM_FADEOUT)
				StopBGM((DEFINE_BGM)i);

			ptr->state = BGM_NORMAL;
		
		
		}
	}

	DrawDebugWindowBGM();
}

/**************************************
再生処理
***************************************/
void PlayBGM(DEFINE_BGM bgm)
{
	SetSoundVolume(bgmData[bgm].clip, bgmData[bgm].volume);
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

/**************************************
フェードイン設定
***************************************/
void FadeInBGM(DEFINE_BGM bgm, int duration, bool isResume)
{
	BGM *ptr = &bgmData[bgm];
	ptr->cntFrame = 0;
	ptr->fadeDuration = duration;
	ptr->state = BGM_FADEIN;

	if (isResume)
		ResumeBGM(bgm);
	else
		PlayBGM(bgm);
}

/**************************************
フェードアウト設定
***************************************/
void FadeOutBGM(DEFINE_BGM bgm, int duration)
{
	BGM *ptr = &bgmData[bgm];
	ptr->cntFrame = 0;
	ptr->fadeDuration = duration;
	ptr->state = BGM_FADEOUT;
}

/**************************************
デバッグウィンドウ
***************************************/
void DrawDebugWindowBGM(void)
{
	BeginDebugWindow("BGM");

	DebugSliderFloat(STR(BGM_TITLESCENE), &bgmData[BGM_TITLESCENE].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);
	DebugSliderFloat(STR(BGM_TUTORIALSCENE), &bgmData[BGM_TUTORIALSCENE].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);
	DebugSliderFloat(STR(BGM_BATTLESCENE), &bgmData[BGM_BATTLESCENE].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);
	DebugSliderFloat(STR(BGM_BONUSTIME), &bgmData[BGM_BONUSTIME].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);
	DebugSliderFloat(STR(BGM_GAMEOVER), &bgmData[BGM_GAMEOVER].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);
	DebugSliderFloat(STR(BGM_STAGECLEAR), &bgmData[BGM_STAGECLEAR].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);
	DebugSliderFloat(STR(BGM_NAMEENTRY), &bgmData[BGM_NAMEENTRY].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);


	if (DebugButton("Save Settings"))
	{
		for (int i = 0; i < BGM_MAX; i++)
		{
			SetSoundVolume(bgmData[i].clip, bgmData[i].volume);
		}
		SaveSettingsBGM();
	}

	EndDebugWindow("BGM");
}

/**************************************
設定保存処理
***************************************/
bool SaveSettingsBGM(void)
{
	FILE *fp = NULL;

	fp = fopen("data/SETTINGS/bgm.ini", "wb");

	if (fp == NULL)
	{
		return false;
	}

	BGM *ptr = &bgmData[0];
	for (int i = 0; i < BGM_MAX;i++, ptr++)
	{
		fwrite(&ptr->volume, sizeof(float), 1, fp);
	}

	fclose(fp);
	return true;
}

/**************************************
設定読み込み処理
***************************************/
bool LoadSettingsBGM(void)
{
	FILE *fp = NULL;

	fp = fopen("data/SETTINGS/bgm.ini", "rb");
	if (fp == NULL)
	{
		BGM *ptr = &bgmData[0];
		for (int i = 0; i < BGM_MAX; i++, ptr++)
		{
			ptr->volume = SOUND_VOLUME_INIT;
		}
		return false;
	}

	BGM *ptr = &bgmData[0];
	for (int i = 0; i < BGM_MAX; i++, ptr++)
	{
		int res = fread(&ptr->volume, sizeof(float), 1, fp);
		if (res == EOF)
			ptr->volume = SOUND_VOLUME_INIT;
	}

	return true;
}
