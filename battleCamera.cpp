//=============================================================================
//
// カメラ処理 [battleCamera.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "battleCamera.h"
#include "input.h"
#include "Easing.h"
#include "debugproc.h"
#include "playerModel.h"
#include "cameraShaker.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	BATTLECAMERA_VIEWANGLE				(D3DXToRadian(45.0f))	// 視野角
#define	BATTLECAMERA_VIEWASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	BATTLECAMERA_NEAR					(50.0f)					// ビュー平面のNearZ値
#define	BATTLECAMERA_FAR					(20000.0f)				// ビュー平面のFarZ値
#define BATTLECAMERA_LENGTH_FPS				(175.0f)
#define BATTLECAMERA_LENGTH_TOP				(250.0f)
#define BATTLECAMERA_LENGTH_SIDE			(200.0f)
#define BATTLECAMERA_LENGTH_QUATER			(200.0f)
#define BATTLECAMERA_TOP_ANGLE_Y			(0.68f)
#define BATTLECAMERA_QUATER_ANGLE_Y			(0.34f)
#define BATTLECAMERA_QUATER_ANGLE_XZ		(1.19f)
#define BATTLECAMERA_SIDE_ANGLE_Y			(0.17f)

#define BATTLECAMERA_USE_QUATERNION			(0)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BATTLECAMERA camera;

#if BATTLECAMERA_USE_QUATERNION
static const D3DXQUATERNION BattleCameraPosQ[] =
{
	//FPS
	D3DXQUATERNION(0.0f, 0.0f, -BATTLECAMERA_LENGTH * 0.7f, 1.0f),
	//縦
	D3DXQUATERNION(0.0f, sinf(BATTLECAMERA_TOP_ANGLE_Y) * BATTLECAMERA_LENGTH , -cosf(BATTLECAMERA_TOP_ANGLE_Y) * BATTLECAMERA_LENGTH, 1.0f),
	//横
	D3DXQUATERNION(BATTLECAMERA_LENGTH, 0.0f, 0.0f, 1.0f),
	//クォーター
	D3DXQUATERNION(
		cosf(BATTLECAMERA_QUATER_ANGLE_Y) * cosf(BATTLECAMERA_QUATER_ANGLE_XZ) * BATTLECAMERA_LENGTH * 0.8f,
		sinf(BATTLECAMERA_QUATER_ANGLE_Y) * BATTLECAMERA_LENGTH,
		cosf(BATTLECAMERA_QUATER_ANGLE_Y) * -sinf(BATTLECAMERA_QUATER_ANGLE_XZ) * BATTLECAMERA_LENGTH * 0.8f,
		1.0f
	)
};
#else
static const D3DXVECTOR3 BattleCameraPos[] =
{
	//FPS
	D3DXVECTOR3(0.0f, 0.0f, -BATTLECAMERA_LENGTH_FPS * 0.7f),
	//縦
	D3DXVECTOR3(0.0f, sinf(BATTLECAMERA_TOP_ANGLE_Y) * BATTLECAMERA_LENGTH_TOP , -cosf(BATTLECAMERA_TOP_ANGLE_Y) * BATTLECAMERA_LENGTH_TOP),
	//横
	D3DXVECTOR3(BATTLECAMERA_LENGTH_SIDE, 0.0f, 0.0f),
	//クォーター
	D3DXVECTOR3(
		cosf(BATTLECAMERA_QUATER_ANGLE_Y) * cosf(BATTLECAMERA_QUATER_ANGLE_XZ) * BATTLECAMERA_LENGTH_QUATER * 0.8f,
		sinf(BATTLECAMERA_QUATER_ANGLE_Y) * BATTLECAMERA_LENGTH_QUATER,
		cosf(BATTLECAMERA_QUATER_ANGLE_Y) * -sinf(BATTLECAMERA_QUATER_ANGLE_XZ) * BATTLECAMERA_LENGTH_QUATER * 0.8f
	)
};
#endif
//static float cameraLength = BATTLECAMERA_LENGTH_TOP;

static const D3DXVECTOR3 BattleCameraAt[] =
{
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
};

static const D3DXVECTOR3 BattleCameraUp[] =
{
	D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	D3DXVECTOR3(0.0f, 1.0f, 0.0f)
};

static D3DXVECTOR3 offsetPos;

//=============================================================================
// カメラの初期化
//=============================================================================
HRESULT InitBattleCamera(void)
{
#if BATTLECAMERA_USE_QUATERNION
	camera.pos.x = BattleCameraPosQ[0].x;
	camera.pos.y = BattleCameraPosQ[0].x;
	camera.pos.z = BattleCameraPosQ[0].z;
#else
	camera.pos.x = BattleCameraPos[0].x;
	camera.pos.y = BattleCameraPos[0].x;
	camera.pos.z = BattleCameraPos[0].z;
#endif
	camera.at = BattleCameraAt[0];
	camera.up = BattleCameraUp[0];

	InitCameraShaker(0);

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitBattleCamera(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateBattleCamera(void)
{
	UpdateCameraShaker();

	if (camera.isMoving)
	{
		camera.cntFrame++;
		float t = EaseOutCubic((float)camera.cntFrame / BATTLECAMERA_MOVEFRAME, 0.0f, 1.0f);
#if BATTLECAMERA_USE_QUATERNION
		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &BattleCameraPosQ[camera.currentState], &BattleCameraPosQ[camera.nextState], t);
		camera.pos.x = q.x;
		camera.pos.y = q.y;
		camera.pos.z = q.z;
#else
		camera.pos.x = EaseOutCubic(t, BattleCameraPos[camera.currentState].x, BattleCameraPos[camera.nextState].x);
		camera.pos.y = EaseOutCubic(t, BattleCameraPos[camera.currentState].y, BattleCameraPos[camera.nextState].y);
		camera.pos.z = EaseOutCubic(t, BattleCameraPos[camera.currentState].z, BattleCameraPos[camera.nextState].z);
#endif
		camera.up.x = EaseLinear(t, BattleCameraUp[camera.currentState].x, BattleCameraUp[camera.nextState].x);
		camera.up.y = EaseLinear(t, BattleCameraUp[camera.currentState].y, BattleCameraUp[camera.nextState].y);
		camera.up.z = EaseLinear(t, BattleCameraUp[camera.currentState].z, BattleCameraUp[camera.nextState].z);

		if (camera.cntFrame == BATTLECAMERA_MOVEFRAME)
		{
			camera.isMoving = false;
			camera.currentState = camera.nextState;
		}
	}

	if (GetKeyboardTrigger(DIK_M))
	{
		SetBattleCameraMove((camera.currentState + 1) % 4);
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetBattleCamera(void)
{
	bool flg = GetKeyboardTrigger(DIK_Q);


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 cameraOffset = GetCameraOffsetValue();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camera.view);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camera.view, 
						&(camera.pos + cameraOffset),		// カメラの視点
						&(camera.at + cameraOffset),			// カメラの注視点
						&camera.up);	
	// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera.view);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&camera.projection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&camera.projection,
								BATTLECAMERA_VIEWANGLE,			// 視野角
								BATTLECAMERA_VIEWASPECT,		// アスペクト比
								BATTLECAMERA_NEAR,		// ビュー平面のNearZ値
								BATTLECAMERA_FAR);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera.projection);
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetBattleCameraView(void)
{
	return camera.view;
}

//=============================================================================
// プロジェクションマトリックスの取得
//=============================================================================
D3DXMATRIX GetBattleCameraProjection(void)
{
	return camera.projection;
}

//=============================================================================
// カメラを向く回転行列の取得
//=============================================================================
void GetInvRotBattleCamera(D3DXMATRIX *mtx, D3DXVECTOR3 *pAt)
{
	if (pAt != NULL)
	{
		D3DXMATRIX inv;
		D3DXMatrixIdentity(mtx);
		D3DXMatrixLookAtLH(mtx, &camera.pos, pAt, &camera.up);
		D3DXMatrixInverse(mtx, NULL, mtx);
		mtx->_41 = 0.0f;
		mtx->_42 = 0.0f;
		mtx->_43 = 0.0f;
	}
	else {
		mtx->_11 = camera.view._11;
		mtx->_12 = camera.view._21;
		mtx->_13 = camera.view._31;

		mtx->_21 = camera.view._12;
		mtx->_22 = camera.view._22;
		mtx->_23 = camera.view._32;

		mtx->_31 = camera.view._13;
		mtx->_32 = camera.view._23;
		mtx->_33 = camera.view._33;

		mtx->_41 = 0.0f;
		mtx->_42 = 0.0f;
		mtx->_43 = 0.0f;
	}
}

//=============================================================================
// カメラ座標の取得
//=============================================================================
D3DXVECTOR3 GetBattleCameraPos(void)
{
	return camera.pos;
}

//=============================================================================
// カメラの移動のセット
//=============================================================================
void SetBattleCameraMove(int state)
{
	if (camera.isMoving)
	{
		return;
	}

	camera.isMoving = true;
	camera.nextState = state;
	camera.cntFrame = 0;

	PLAYERMODEL *player = GetPlayerAdr(0);
	for (int i = 0; i < PLAYERMODEL_MAX; i++, player++)
	{
		player->nextState = camera.nextState;
	}
	ChangeStatePlayerModel(PlayerTransition);
}

//=============================================================================
// バトルカメラ取得関数
//=============================================================================
BATTLECAMERA *GetBattleCameraAdr(void)
{
	return &camera;
}


//=============================================================================
// カメラの状態セット
//=============================================================================
void SetBattleCameraState(int state)
{
	camera.isMoving = false;
	camera.currentState = state;
	camera.pos = BattleCameraPos[state];
	camera.at = BattleCameraAt[state];
	camera.up = BattleCameraUp[state];
}
