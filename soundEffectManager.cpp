//=====================================
//
//サウンドエフェクトマネージャ処理[soundEffectManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "soundEffectManager.h"
#include "sound.h"
#include <tchar.h>
#include "debugWindow.h"
#include <stdio.h>

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

/**************************************
マクロ定義
***************************************/
#define SOUND_POS_FAR_END			(10000.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
const TCHAR* soundFileName[SOUND_MAX] =
{
	_T("data/SOUND/lockon.wav"),
	_T("data/SOUND/burst00.wav"),
	_T("data/SOUND/don18_B.wav"),
	_T("data/SOUND/ready.wav"),
	_T("data/SOUND/decision16.wav"),
	_T("data/SOUND/gun29.wav"),
	//_T("data/SOUND/hoge.wav"),
};

static SOUNDEFFECT se[SOUND_MAX];

/**************************************
プロトタイプ宣言
***************************************/
bool SaveSettingsSoundEffect(void);
bool LoadSettingsSoundEffect(void);
void DrawDebugWindowSoundEffect(void);

/**************************************
初期化処理
***************************************/
void InitSoundEffectManager(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		bool res = LoadSettingsSoundEffect();
		SOUNDEFFECT *ptr = &se[0];
		for (int i = 0; i < SOUND_MAX; i++, ptr++)
		{
			ptr->clip = LoadSound(&soundFileName[i][0]);
		}

		initialized = true;
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
		SOUNDEFFECT *ptr = &se[0];
		for (int i = 0; i < SOUND_MAX; i++, ptr++)
		{
			SAFE_RELEASE(ptr->clip);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateSoundEffectManager(void)
{
	DrawDebugWindowSoundEffect();
}

/**************************************
再生処理
***************************************/
void PlaySE(DEFINE_SOUNDEFFECT sound)
{
	PlaySoundBuffer(se[sound].clip, E_DS8_FLAG_NONE, true);
}

/**************************************
再生処理(3D版)
***************************************/
void PlaySE_3D(DEFINE_SOUNDEFFECT sound, float posZ)
{
	float decay = 1.0f - posZ / SOUND_POS_FAR_END;
	SetSoundVolume(se[sound].clip, se[sound].volume * decay);
	PlaySoundBuffer(se[sound].clip, E_DS8_FLAG_NONE, true);
}

/**************************************
停止処理
***************************************/
void StopSE(DEFINE_SOUNDEFFECT sound)
{
	StopSoundBuffer(se[sound].clip);
}

/**************************************
再開処理
***************************************/
void ResumeSE(DEFINE_SOUNDEFFECT sound)
{
	PlaySoundBuffer(se[sound].clip, E_DS8_FLAG_NONE, false);
}

/**************************************
再生中判定
***************************************/
bool IsPlayingSE(DEFINE_SOUNDEFFECT sound)
{
	return IsPlaying(se[sound].clip);
}

/**************************************
音量設定
***************************************/
void SetSEVolume(DEFINE_SOUNDEFFECT sound, float volume)
{
	SetSoundVolume(se[sound].clip, volume);
}

/**************************************
デバッグウィンドウ
***************************************/
void DrawDebugWindowSoundEffect(void)
{
	BeginDebugWindow("SoundEffect");

	DebugSliderFloat(STR(SOUND_LOCKON), &se[SOUND_LOCKON].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);

	DebugSliderFloat(STR(SOUND_MISSILELAUNCH), &se[SOUND_MISSILELAUNCH].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);

	DebugSliderFloat(STR(SOUND_SMALLEXPL), &se[SOUND_SMALLEXPL].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);

	DebugSliderFloat(STR(SOUND_READY), &se[SOUND_READY].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);

	DebugSliderFloat(STR(SOUND_DECISION), &se[SOUND_DECISION].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);

	DebugSliderFloat(STR(SOUND_SHOT), &se[SOUND_SHOT].volume, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX);


	if (DebugButton("Save Settings"))
	{
		SaveSettingsSoundEffect();
		for (int i = 0; i < SOUND_MAX; i++)
		{
			SetSoundVolume(se[i].clip, se[i].volume);
		}
	}

	EndDebugWindow("SoundEffect");

	BeginDebugWindow("PlaySound");

	if (DebugButton(STR(SOUND_LOCKON)))	{ PlaySE(SOUND_LOCKON); }
	if (DebugButton(STR(SOUND_MISSILELAUNCH))) { PlaySE(SOUND_MISSILELAUNCH); }
	if (DebugButton(STR(SOUND_SMALLEXPL))) { PlaySE(SOUND_SMALLEXPL); }
	if (DebugButton(STR(SOUND_READY))) { PlaySE(SOUND_READY); }
	if (DebugButton(STR(SOUND_DECISION))) { PlaySE(SOUND_DECISION); }
	if (DebugButton(STR(SOUND_SHOT))) { PlaySE(SOUND_SHOT); }

	DebugNewLine();
	static float length = 5000.0f;
	DebugSliderFloat("Length", &length, 0.0f, SOUND_POS_FAR_END);
	if (DebugButton("Play 3D"))
	{
		PlaySE_3D(SOUND_SMALLEXPL, length);
	}


	EndDebugWindow("PlaySound");
}

/**************************************
設定保存処理
***************************************/
bool SaveSettingsSoundEffect(void)
{
	FILE *fp = NULL;
	fp = fopen("data/SETTINGS/sound.ini", "wb");

	if (fp == NULL)
	{
		return false;
	}

	for (int i = 0; i < SOUND_MAX; i++)
	{
		fwrite(&se[i].volume, sizeof(float), 1, fp);
	}

	fclose(fp);

	return true;
}

/**************************************
設定読み込み処理
***************************************/
bool LoadSettingsSoundEffect(void)
{
	FILE *fp = NULL;
	fp = fopen("data/SETTINGS/sound.ini", "rb");

	if (fp == NULL)
	{
		SOUNDEFFECT *ptr = &se[0];
		for (int i = 0; i < SOUND_MAX; i++, ptr++)
		{
			ptr->volume = SOUND_VOLUME_INIT;
		}
		return false;
	}

	for (int i = 0; i < SOUND_MAX; i++)
	{
		int res = fread(&se[i].volume, sizeof(float), 1, fp);
		if (res == EOF)
			se[i].volume = SOUND_VOLUME_INIT;
	}

	fclose(fp);

	return true;
}