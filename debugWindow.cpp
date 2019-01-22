//=====================================
//
//テンプレート処理[template.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "debugWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "input.h"

/**************************************
マクロ定義
***************************************/
#define INTERBAL_GETTIMER		(20)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
LARGE_INTEGER timeCountBegin;
static unsigned int cntFrame = 0;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitDebugWindow(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	ImGui::StyleColorsDark();
}

/**************************************
終了処理
***************************************/
void UninitDebugWindow(int num)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

/**************************************
更新処理
***************************************/
void UpdateDebugWindow(void)
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	cntFrame++;
}

/**************************************
描画処理
***************************************/
void DrawDebugWindow(void)
{
	static bool enableDraw = true;

	if (GetKeyboardTrigger(DIK_D) && GetKeyboardPress(DIK_LCONTROL))
		enableDraw = !enableDraw;

	if (!enableDraw)
	{
		ImGui::EndFrame();
		return;
	}
	
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

/**************************************
高解像度タイマーでの計測開始処理
***************************************/
void BeginTimerCount(void)
{
	QueryPerformanceCounter(&timeCountBegin);
}

/**************************************
計測開始からの経過時間[単位：msec]
***************************************/
double GetProgressTimerCount(void)
{
	//タイマーの周波数を取得
	LARGE_INTEGER frequencyTimer;
	QueryPerformanceFrequency(&frequencyTimer);

	//カウント取得
	LARGE_INTEGER timeCurrent;
	QueryPerformanceCounter(&timeCurrent);

	//計測開始からの経過時間[msec]を計算
	LONGLONG span = timeCurrent.QuadPart - timeCountBegin.QuadPart;
	double msec = (double)span * 1000 / (double)frequencyTimer.QuadPart;

	return msec;
}

/**************************************
タイマーカウント取得処理（20フレームおきに動作）
***************************************/
void GetTimerCount(LARGE_INTEGER *ptr)
{
	if (cntFrame % INTERBAL_GETTIMER != 0)
		return;

	QueryPerformanceCounter(ptr);
}

/**************************************
経過時間取得処理[msec]
***************************************/
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end)
{
	//タイマーの周波数取得
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	//経過時間を計算
	LONGLONG span = end.QuadPart - start.QuadPart;
	double msec = (double)span * 1000.0f / (double)frequency.QuadPart;

	return msec;
}

/*************************************
デバッグウィンドウ開始処理
***************************************/
void BeginDebugWindow(const char *label)
{
	ImGui::Begin(label);
}

/*************************************
デバッグウィンドウ終了処理
***************************************/
void EndDebugWindow(const char *label)
{
	ImGui::End();
}

/*************************************
デバッグテキスト表示処理
***************************************/
void DebugText(const char *str, ...)
{
	va_list ap;
	va_start(ap, str);
	ImGui::TextV(str, ap);
	//ImGui::Text(str, ap);
	va_end(ap);
}

/*************************************
デバッグボタン表示処理
***************************************/
bool DebugButton(const char *label)
{
	return ImGui::Button(label);
}

/*************************************
デバッグスライダー処理
***************************************/
void DebugSliderFloat(const char *label, float *adr, float min, float max)
{
	ImGui::SliderFloat(label, adr, min, max);
}

/*************************************
デバッグカラーピッカー処理
***************************************/
void DebugColorEditor(const char *label, float array[4])
{
	ImGui::ColorEdit4(label, array);
}

/*************************************
デバッグウィンドウ改行処理
***************************************/
void DebugNewLine(void)
{
	ImGui::NewLine();
}

/*************************************
ツリー構造展開処理
***************************************/
void DebugTreeExpansion(bool isOpen)
{
	ImGui::SetNextTreeNodeOpen(isOpen, ImGuiSetCond_Once);
}

/*************************************
ツリー構造プッシュ処理
***************************************/
bool DebugTreePush(const char *label)
{
	return ImGui::TreeNode(label);
}

/*************************************
ツリー構造ポップ処理
***************************************/
void DebugTreePop(void)
{
	ImGui::TreePop();
}