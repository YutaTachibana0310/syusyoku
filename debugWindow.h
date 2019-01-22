//=====================================
//
//デバッグウィンドウヘッダ[debugWindow.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _DEBUGWINDOW_H_
#define _DEBUGWINDOW_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define USE_DEBUGWINDOW
#define STR(var) #var		//varを文字列に置換するマクロ
/**************************************
構造体定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
LRESULT DebugWindPrcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitDebugWindow(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
void UninitDebugWindow(int num);
void UpdateDebugWindow(void);
void DrawDebugWindow(void);

void BeginTimerCount(void);				//高解像度タイマー計測開始
double GetProgressTimerCount(void);		//タイマー経過時間取得処理
void GetTimerCount(LARGE_INTEGER *ptr);	//タイマーカウント取得(20フレーム間隔)
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end);	//経過時間取得	

//ImGUIラッパー関数
void BeginDebugWindow(const char *label);
void EndDebugWindow(const char* label);
void DebugText(const char *str, ...);
bool DebugButton(const char *label);
void DebugSliderFloat(const char *label, float *adr, float min, float max);
void DebugColorEditor(const char *label, float array[4]);
void DebugNewLine(void);
void DebugTreeExpansion(bool isOpen);
bool DebugTreePush(const char *label);
void DebugTreePop(void);

#endif