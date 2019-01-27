//=====================================
//
//パーティクルマネージャ処理[particleManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "particleManager.h"
#include "explosionFire.h"
#include "explosionFlare.h"
#include "explosionSmog.h"
#include "camera.h"
#include "cubeParticle.h"
#include "explosionLumine.h"
#include "bulletParticle.h"
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define PARTICLE_SHADER_PATH	"particle.fx"

#define PARTICLE_EMMITT_NUM_FIRE		(10)
#define PARTICLE_EMMITT_NUM_SMOG		(10)
#define PARTICLE_EMMITT_NUM_FLARE		(20)
#define PARTICLE_EMMITT_NUM_CUBE		(40)
#define PARTICLE_EMMITT_NUM_LUMINE		(20)
#define PARTICLE_EMMITT_NUM_PLAYERBULLET (10)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXDECLARATION9 declare = NULL;
static LPD3DXEFFECT effect = NULL;

static LARGE_INTEGER startFlare, endFlare;
static LARGE_INTEGER startSmog, endSmog;
static LARGE_INTEGER startFire, endFire;

static D3DXCOLOR cubeColor;
/**************************************
プロトタイプ宣言
***************************************/
void DrawDebugWindowParticle(void);

/**************************************
初期化処理
***************************************/
void InitParticleManager(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static bool initialized = false;

	if (!initialized)
	{
		D3DVERTEXELEMENT9 declareElems[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
			{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
			{ 3, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
			D3DDECL_END()
		};
		pDevice->CreateVertexDeclaration(declareElems, &declare);

		D3DXCreateEffectFromFile(pDevice, PARTICLE_SHADER_PATH, 0, 0, 0, 0, &effect, NULL);

		initialized = true;
	}

	InitExplosionSmog(num);
	InitExplosionFire(num);
	InitExplosionFlare(num);
	InitCubeParticle(num);
	InitExplosionLumine(num);
	InitBulletParticle(num);
}

/**************************************
終了処理
***************************************/
void UninitParticleManager(int num)
{
	UninitExplosionSmog(num);
	UninitExplosionFire(num);
	UninitExplosionFlare(num);
	UninitCubeParticle(num);
	UninitExplosionLumine(num);
	UninitBulletParticle(num);
}

/**************************************
更新処理
***************************************/
void UpdateParticleManager(void)
{
	GetTimerCount(&startSmog);
	UpdateExplosionSmog();
	GetTimerCount(&endSmog);

	GetTimerCount(&startFire);
	UpdateExplosionFire();
	GetTimerCount(&endFire);

	GetTimerCount(&startFlare);
	UpdateExplosionFlare();
	GetTimerCount(&endFlare);

	UpdateCubeParticle();

	UpdateExplosionLumine();

	UpdateBulletParticle();

	DrawDebugWindowParticle();
}

/**************************************
描画処理
***************************************/
void DrawParticleManager(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//頂点要素宣言
	pDevice->SetVertexDeclaration(declare);

	DrawExplosionSmog(declare, effect);
	DrawExplosionFlare(declare, effect);
	DrawExplosionFire(declare, effect);
	DrawBulletParticle(declare, effect);
	DrawExplosionLumine(effect);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GetDevice()->SetRenderState(D3DRS_LIGHTING, true);

	DrawCubeParticle();
}

/**************************************
エネミーエクスプロージョンセット処理
***************************************/
void SetEnemyExplosion(D3DXVECTOR3 pos)
{
	bool result = false;
	for (int j = 0; j < PARTICLE_EMMITT_NUM_FLARE; j++)
	{
		result = SetExplosionFlare(&pos);

		if (!result)
			break;
	}

	for (int j = 0; j < PARTICLE_EMMITT_NUM_FIRE; j++)
	{
		SetExplosionFire(&pos);

	}
	for (int j = 0; j < PARTICLE_EMMITT_NUM_SMOG; j++)
	{
		SetExplosionSmog(&pos);
	}
}

/*************************************
キューブエクスプロージョンセット処理
***************************************/
void SetCubeExplosion(D3DXVECTOR3 pos, D3DXCOLOR color)
{
	for (int i = 0; i < PARTICLE_EMMITT_NUM_CUBE; i++)
	{
		SetCubeParticle(pos, color);
	}

	for (int i = 0; i < PARTICLE_EMMITT_NUM_LUMINE; i++)
	{
		SetExplosionLumine(&pos);
	}
}

/**************************************
プレイヤーバレットエクスプロージョンセット処理
***************************************/
void SetPlayerBulletExplosion(D3DXVECTOR3 pos)
{
	for (int i = 0; i < PARTICLE_EMMITT_NUM_PLAYERBULLET; i++)
	{
		SetExplosionLumine(&pos);
	}
}

/**************************************
デバッグ情報表示
***************************************/
void DrawDebugWindowParticle(void)
{
	//ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f));
	//ImGui::SetNextWindowPos(ImVec2(5.0f, 415.0f));
	BeginDebugWindow("ParticleManager");

	DebugText("Flare  : %fmsec", CalcProgressTime(startFlare, endFlare));
	DebugText("Fire   : %fmsec", CalcProgressTime(startFire, endFire));
	DebugText("Smog   : %fmsec", CalcProgressTime(startSmog, endSmog));

	DebugNewLine();
	DebugColorEditor("Cube Particle", cubeColor);

	EndDebugWindow("ParticleManager");
}