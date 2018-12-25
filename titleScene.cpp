//=============================================================================
//
// タイトルシーン処理 [titleScene.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "titleScene.h"
#include "input.h"
#include "Easing.h"
#include "meshCylinder.h"
#include "playerModel.h"
#include "playerBullet.h"
#include "playerBulletTrail.h"
#include "battleCamera.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TITLESCENE_FADEIN_END		(60)

#define TITLESCENE_LOGOTEX_NAME		"data/TEXTURE/UI/titlelogo00.png"
#define TITLESCENE_LOGOTEX_SIZE_X	(600)
#define TITLESCENE_LOGOTEX_SIZE_Y	(208)
#define TITLESCENE_LOGOTEX_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f))

#define TITLESCENE_BGTEX_NAME		"data/TEXTURE/UI/titlebg00.png"
#define TITLESCENE_BGTEX_SIZE_X		(900)
#define TITLESCENE_BGTEX_SIZE_Y		(500)
#define TITLESCENE_BGTEX_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))

#define TITLESCENE_ANIMTEX_NAME		"data/TEXTURE/UI/titleanim00.png"
#define TITLESCENE_ANIMTEX_SIZE_X	(900)
#define TITLESCENE_ANIMTEX_SIZE_Y	(500)
#define TITLESCENE_ANIMTEX_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTitleScene(void);			//頂点作成関数

void SetTextureTitleLogo(void);				// テクスチャ座標の計算処理
void SetTextureTitleBG(void);				// テクスチャ座標の計算処理
void SetTextureTitleAnim(void);				// テクスチャ座標の計算処理

void SetVertexTitleLogo(void);				// 頂点の計算処理
void SetVertexTitleBG(void);				// 頂点の計算処理
void SetVertexTitleAnim(void);				// 頂点の計算処理

void SetTitleTextureAlpha(float alpha);		//アルファ設定処理

/*****************************************************************************
構造体定義
*****************************************************************************/
enum TITLESCENE_STATE
{
	TITLESCENE_FADEIN,
	TITLESCENE_STATEMAX
};
/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 logoTex, bgTex, animTex;
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static float logoRadius, bgRadius, animRadius;
static float logoAngle, bgAngle, animAngle;
static D3DXVECTOR3 logoPos, bgPos, animPos;
static int cntFrame;
static TITLESCENE_STATE state;

/******************************************s************************************
初期化処理
******************************************************************************/
HRESULT InitTitleScene(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ロゴの初期設定
	logoPos = TITLESCENE_LOGOTEX_POS;
	logoAngle = atan2f(TITLESCENE_LOGOTEX_SIZE_Y, TITLESCENE_LOGOTEX_SIZE_X);
	logoRadius = D3DXVec2Length(&D3DXVECTOR2(TITLESCENE_LOGOTEX_SIZE_X, TITLESCENE_LOGOTEX_SIZE_Y));

	//背景の初期設定
	bgPos = TITLESCENE_BGTEX_POS;
	bgAngle = atan2f(TITLESCENE_BGTEX_SIZE_Y, TITLESCENE_BGTEX_SIZE_X);
	bgRadius = D3DXVec2Length(&D3DXVECTOR2(TITLESCENE_BGTEX_SIZE_X, TITLESCENE_BGTEX_SIZE_Y));

	//アニメテクスチャの初期化
	animPos = TITLESCENE_ANIMTEX_POS;
	animAngle = atan2f(TITLESCENE_ANIMTEX_SIZE_Y, TITLESCENE_ANIMTEX_SIZE_X);
	animRadius = D3DXVec2Length(&D3DXVECTOR2(TITLESCENE_ANIMTEX_SIZE_X, TITLESCENE_ANIMTEX_SIZE_Y));

	// 頂点情報の作成
	MakeVertexTitleScene();

	if (num == 0)
	{
		// テクスチャの読み込み
		logoTex = CreateTextureFromFile((LPSTR)TITLESCENE_LOGOTEX_NAME, pDevice);
		bgTex = CreateTextureFromFile((LPSTR)TITLESCENE_BGTEX_NAME, pDevice);
		animTex = CreateTextureFromFile((LPSTR)TITLESCENE_ANIMTEX_NAME, pDevice);
	}

	if (num != 0)
	{
		SetBackColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetTitleTextureAlpha(0.0f);

		InitMeshCylinder(num);
		InitPlayerModel(num);
		InitPlayerBullet(num);
		UninitPlayerBulletTrail(num);

		ChangeStatePlayerModel(PlayerTitle);
		SetBattleCameraState(FirstPersonCamera);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTitleScene(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(logoTex);
		SAFE_RELEASE(bgTex);
		SAFE_RELEASE(animTex);
	}
	else
	{
		UninitMeshCylinder(num);
		UninitPlayerModel(num);
		UninitPlayerBullet(num);
		UninitPlayerBulletTrail(num);
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateTitleScene(void)
{
	
	if (state == TITLESCENE_FADEIN)
	{
		cntFrame++;
		float t = (float)cntFrame / TITLESCENE_FADEIN_END;

		SetTitleTextureAlpha(EaseLinear(t, 0.0f, 1.0f));
		if (cntFrame == TITLESCENE_FADEIN_END)
		{
			state = TITLESCENE_STATEMAX;
		}
	}

	if (GetKeyboardTrigger(DIK_Z))
	{
		SetScene(BattleScene);
	}

	UpdateMeshCylinder();
	UpdatePlayerModel();
	UpdatePlayerBullet();
	UpdatePlayerBulletTrail();
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTitleScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SetBattleCamera();

	DrawMeshCylinder();
	DrawPlayerModel();
	DrawPlayerBullet();
	DrawPlayerBulletTrail();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	////アニメテクスチャ描画
	//pDevice->SetTexture(0, animTex);
	//SetVertexTitleAnim();
	//SetTextureTitleAnim();
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	//
	////背景描画
	//pDevice->SetTexture(0, bgTex);
	//SetVertexTitleBG();
	//SetTextureTitleBG();
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//ロゴ描画
	pDevice->SetTexture(0, logoTex);
	SetVertexTitleLogo();
	SetTextureTitleLogo();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTitleScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureTitleLogo(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureTitleBG(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureTitleAnim(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTitleLogo(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = logoPos.x - cosf(logoAngle) * logoRadius;
	vertexWk[0].vtx.y = logoPos.y - sinf(logoAngle) * logoRadius;
	vertexWk[1].vtx.x = logoPos.x + cosf(logoAngle) * logoRadius;
	vertexWk[1].vtx.y = logoPos.y - sinf(logoAngle) * logoRadius;
	vertexWk[2].vtx.x = logoPos.x - cosf(logoAngle) * logoRadius;
	vertexWk[2].vtx.y = logoPos.y + sinf(logoAngle) * logoRadius;
	vertexWk[3].vtx.x = logoPos.x + cosf(logoAngle) * logoRadius;
	vertexWk[3].vtx.y = logoPos.y + sinf(logoAngle) * logoRadius;
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTitleBG(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = bgPos.x - cosf(bgAngle) * bgRadius;
	vertexWk[0].vtx.y = bgPos.y - sinf(bgAngle) * bgRadius;
	vertexWk[1].vtx.x = bgPos.x + cosf(bgAngle) * bgRadius;
	vertexWk[1].vtx.y = bgPos.y - sinf(bgAngle) * bgRadius;
	vertexWk[2].vtx.x = bgPos.x - cosf(bgAngle) * bgRadius;
	vertexWk[2].vtx.y = bgPos.y + sinf(bgAngle) * bgRadius;
	vertexWk[3].vtx.x = bgPos.x + cosf(bgAngle) * bgRadius;
	vertexWk[3].vtx.y = bgPos.y + sinf(bgAngle) * bgRadius;
}

/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTitleAnim(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = animPos.x - cosf(animAngle) * animRadius;
	vertexWk[0].vtx.y = animPos.y - sinf(animAngle) * animRadius;
	vertexWk[1].vtx.x = animPos.x + cosf(animAngle) * animRadius;
	vertexWk[1].vtx.y = animPos.y - sinf(animAngle) * animRadius;
	vertexWk[2].vtx.x = animPos.x - cosf(animAngle) * animRadius;
	vertexWk[2].vtx.y = animPos.y + sinf(animAngle) * animRadius;
	vertexWk[3].vtx.x = animPos.x + cosf(animAngle) * animRadius;
	vertexWk[3].vtx.y = animPos.y + sinf(animAngle) * animRadius;
}
/******************************************************************************
テクスチャアルファ設定
******************************************************************************/
void SetTitleTextureAlpha(float alpha)
{
	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}