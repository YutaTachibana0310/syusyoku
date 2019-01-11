//=============================================================================
//
// メイン処理 [main.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "debugproc.h"
#include <time.h>
#include "particleManager.h"
#include "sceneManager.h"
#include "collider.h"
#include "sound.h"
#include "soundEffectManager.h"
#include "debugWindow.h"
#include "dataContainer.h"
#include "collisionManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"		// ウインドウのクラス名
#define WINDOW_NAME		"影表示処理"		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawDebugWindowMain(void);
//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
static D3DXCOLOR backColor = D3DCOLOR_RGBA(0, 0, 0, 0);
int					g_nCountFPS;			// FPSカウンタ
#ifdef _DEBUG
DefineScene startScene = BattleScene;
#endif
bool				g_bDispDebug = true;	// デバッグ表示ON/OFF
static bool flgPause = false;

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						0,//CW_USEDEFAULT,
						0,//CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif

				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	srand((unsigned)time(NULL));

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用n
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(D3DCAPS9));
	g_pD3DDevice->GetDeviceCaps(&caps);

#if 1
	if ((caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) != 0)
	{
		FLOAT StartPos = 2000;
		FLOAT EndPos = 15000;

		g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, true);
		g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, backColor);
		g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));
		g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));
	}
#endif

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// デバッグ表示処理の初期化
	InitDebugProc();

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	//シーンマネージャ初期化
	InitSceneManager(0);

	//パーティクルマネージャ初期化
	InitParticleManager(0);

	//当たり判定初期化
	InitCollider(0);
	
	//コリージョンマネージャ初期化
	InitCollisionManager(0);

	//サウンド関連初期化
	InitSound(hWnd);
	InitSoundEffectManager(0);

	//データコンテナ初期化
	InitDataContainer(0);

	//デバッグウィンドウ初期化
#ifdef USE_DEBUGWINDOW
	InitDebugWindow(hWnd, g_pD3DDevice);
#endif

#ifdef _DEBUG
	SetScene(startScene);
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力処理の終了処理
	UninitInput();

	// デバッグ表示処理の終了処理
	UninitDebugProc();

	//シーンマネージャ終了処理
	UninitSceneManager(0);

	//パーティクルマネージャ終了処理
	UninitParticleManager(0);

	//当たり判定終了処理
	UninitCollider(0);

	UninitCollisionManager(0);

	//サウンド関連終了処理
	UninitSoundEffectManager(0);
	UninitSound();

#ifdef USE_DEBUGWINDOW
	UninitDebugWindow(0);
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{
		flgPause = !flgPause;
	}

	// 入力更新
	UpdateInput();

	UpdateDebugWindow();

	if (flgPause)
	{
		return;
	}

	UpdateCamera();
	UpdateLight();
	UpdateSceneManager();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), backColor, 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		DrawSceneManager();

		// デバッグ表示処理の描画
		if(g_bDispDebug)
		{
			DrawDebugProc();
		}

		DrawDebugWindowMain();

#ifdef USE_DEBUGWINDOW
		DrawDebugWindow();
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

/******************************************************************************
//関数名	：LPDIRECT3DTEXTURE9 CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice)
//引数1		：LPSTR szName：読み込みたいファイル名
//引数2		：LPDIRECT3DDEVICE9 lpD3DDevice：デバイスオブジェクト
//戻り値	：読み込んだテクスチャへのポインタ
//説明		：サイズを指定してテクスチャファイルを読み込む関数
******************************************************************************/
LPDIRECT3DTEXTURE9 CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice)
{
	HRESULT             hRet;

	//戻り値のテクスチャ
	LPDIRECT3DTEXTURE9  lpTex = NULL;
	if (lpD3DDevice) {

		//D3DXGetImageInfoFromFileで使用する画像ファイルの情報
		D3DXIMAGE_INFO      iinfo;

		//画像ファイルの情報を取得
		hRet = D3DXGetImageInfoFromFile(szName, &iinfo);
		if (hRet == D3D_OK)
		{
			//テクスチャをファイルから作成
			hRet = D3DXCreateTextureFromFileEx(
				lpD3DDevice,
				szName,
				iinfo.Width, iinfo.Height,			//取得した画像ファイルのサイズを指定する
				1, 0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				0xFF000000,
				NULL, NULL,
				&lpTex);
		}
	}
	return lpTex;
}

//=============================================================================
// ビューポート変換
//=============================================================================
void TranslateViewPort(D3DXVECTOR3 *out, D3DXVECTOR3 *pos)
{
	out->x = pos->x * (SCREEN_WIDTH / 2.0f) + SCREEN_WIDTH / 2.0f;
	out->y = pos->y * (-SCREEN_HEIGHT / 2.0f) + SCREEN_HEIGHT / 2.0f;
}

//=============================================================================
// バックカラー変更
//=============================================================================
void SetBackColor(D3DXCOLOR color)
{
	backColor = color;
	g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, backColor);
}

//=============================================================================
// デバッグウィンドウ表示
//=============================================================================
void DrawDebugWindowMain(void)
{
	ImGui::SetNextWindowSize(ImVec2(200.0f, 200.0f));
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - 205.0f, 5.0f));
	ImGui::Begin("Main");

	ImGui::Text("FPS : %d", g_nCountFPS);

	ImGui::End();
}