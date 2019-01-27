//=====================================
//
//ハードキューブオブジェクト処理[hardCubeObject.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hardCubeObject.h"
#include "targetSite.h"
#include "particleManager.h"
#include "dataContainer.h"
#include "collisionManager.h"
#include "battleCamera.h"
#include "playerModel.h"
#include "cubeObject.h"
#include "cameraShaker.h"
#include "soundEffectManager.h"
#include "shockBlur.h"

#include "stageData.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_EFFECT_NAME			"data/EFFECT/cubeObject.fx"	//シェーダ名
#define HARDCUBE_SIZE					(10.0f)						//オブジェクトサイズ
#define HARDCUBE_NUM_MAX				(128)						//最大数
#define HARDCUBE_VTX_NUM				(24)						//頂点数
#define HARDCUBE_FIELD_NUM				(6)							//面数
#define HARDCUBE_TEX_NUM				(3)							//テクスチャ数
#define HARDCUBE_INIT_HP				(10.0f)						//初期HP
#define BONUSCUBE_CAMERA_SHAKE_LENGTH	(10.0f)						//撃破時のカメラの揺れ幅
#define HARDCUBE_SPEED_MAX				(10.0f)						//最大スピード
#define HARDCUBE_SPEED_MIN				(5.0f)						//最小スピード
#define PARTICLE_HARDCUBE_COLOR			(D3DCOLOR_RGBA(255, 121, 121, 255))

//テクスチャ名
static const char* TextureName[HARDCUBE_TEX_NUM] = {
	"data/TEXTURE/OBJECT/circuit06.png",
	"data/TEXTURE/OBJECT/circuit07.png",
	"data/TEXTURE/OBJECT/circuit08.png",
};

/**************************************
構造体定義
***************************************/
typedef struct {
	float posX, posY, posZ;
	float texU, texV;
	float norX, norY, norZ;
}HARDCUBE_VTX;

typedef void(*funcHardCube)(HARD_CUBE_OBJECT *ptr);

/**************************************
グローバル変数
***************************************/
//単位頂点
static HARDCUBE_VTX vtx[HARDCUBE_VTX_NUM] = {
	//上
	{ -HARDCUBE_SIZE, HARDCUBE_SIZE, HARDCUBE_SIZE, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ HARDCUBE_SIZE, HARDCUBE_SIZE, HARDCUBE_SIZE, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ -HARDCUBE_SIZE, HARDCUBE_SIZE,-HARDCUBE_SIZE, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	{ HARDCUBE_SIZE, HARDCUBE_SIZE,-HARDCUBE_SIZE, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	//前
	{ -HARDCUBE_SIZE, HARDCUBE_SIZE,-HARDCUBE_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	{ HARDCUBE_SIZE, HARDCUBE_SIZE,-HARDCUBE_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	{ -HARDCUBE_SIZE,-HARDCUBE_SIZE,-HARDCUBE_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f },
	{ HARDCUBE_SIZE,-HARDCUBE_SIZE,-HARDCUBE_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f },
	//下
	{ -HARDCUBE_SIZE,-HARDCUBE_SIZE,-HARDCUBE_SIZE, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
	{ HARDCUBE_SIZE,-HARDCUBE_SIZE,-HARDCUBE_SIZE, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
	{ -HARDCUBE_SIZE,-HARDCUBE_SIZE, HARDCUBE_SIZE, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
	{ HARDCUBE_SIZE,-HARDCUBE_SIZE, HARDCUBE_SIZE, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
	//後ろ
	{ HARDCUBE_SIZE, HARDCUBE_SIZE, HARDCUBE_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ -HARDCUBE_SIZE, HARDCUBE_SIZE, HARDCUBE_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ HARDCUBE_SIZE,-HARDCUBE_SIZE, HARDCUBE_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	{ -HARDCUBE_SIZE,-HARDCUBE_SIZE, HARDCUBE_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	//左
	{ -HARDCUBE_SIZE, HARDCUBE_SIZE, HARDCUBE_SIZE, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ -HARDCUBE_SIZE, HARDCUBE_SIZE,-HARDCUBE_SIZE, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ -HARDCUBE_SIZE,-HARDCUBE_SIZE, HARDCUBE_SIZE, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	{ -HARDCUBE_SIZE,-HARDCUBE_SIZE,-HARDCUBE_SIZE, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	//右
	{ HARDCUBE_SIZE, HARDCUBE_SIZE,-HARDCUBE_SIZE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ HARDCUBE_SIZE, HARDCUBE_SIZE, HARDCUBE_SIZE, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ HARDCUBE_SIZE,-HARDCUBE_SIZE,-HARDCUBE_SIZE, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ HARDCUBE_SIZE,-HARDCUBE_SIZE, HARDCUBE_SIZE, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
};

static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff;		//頂点バッファ
static LPDIRECT3DVERTEXDECLARATION9 declare;			//頂点宣言
static LPDIRECT3DTEXTURE9 texture[HARDCUBE_TEX_NUM];	//テクスチャ
static LPD3DXEFFECT effect;								//シェーダ
static LPDIRECT3DINDEXBUFFER9 indexBuff;				//インデックスバッファ

static D3DXMATRIX mtxWorld[HARDCUBE_NUM_MAX];			//ワールド変換行列
static HARD_CUBE_OBJECT cube[HARDCUBE_NUM_MAX];			//ハードキューブ配列
static OBJECT_FOR_TREE objectForTree[HARDCUBE_NUM_MAX];	//衝突判定用OBJECT_FOR_TREE配列

//入場処理関数テーブル
static funcHardCube Enter[HardCubeStateMax] = {
	OnEnterHardCubeInit,
	OnEnterHardCubeMove,
	OnEnterHardCubeNormalAttack,
	OnEnterHardCubeHomingAttack,
	OnEnterHardCubeCharge,
	OnEnterHardCubeEscape,
	OnEnterHardCubeBezier,
	OnEnterHardCubeTutorial
};

//更新処理関数テーブル
static funcHardCube Update[HardCubeStateMax] = {
	OnUpdateHardCubeInit,
	OnUpdateHardCubeMove,
	OnUpdateHardCubeNormalAttack,
	OnUpdateHardCubeHomingAttack,
	OnUpdateHardCubeCharge,
	OnUpdateHardCubeEscape,
	OnUpdateHardCubeBezier,
	OnUpdateHardCubeTutorial
};

/**************************************
プロトタイプ宣言
***************************************/
void MoveHardCube(void);						//移動処理
void RotateHardCube(void);						//回転処理
void CalcHardCubeWorldMatrix(void);				//ワールド変換行列計算処理
void CheckDestroyHardCube(void);				//死亡判定	
void RegisterHardCubeToSpace(void);				//衝突空間への登録処理
void OnUpdateHardCube(void);					//各状態更新処理

/**************************************
初期化処理
***************************************/
void InitHardCubeObject(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//キューブ生成範囲
	const float PosRange = 50.0f;
	const float RotRange = 5.0f;

	//パラメータ初期化
	HARD_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		ptr->pos = D3DXVECTOR3(RandomRangef(-PosRange, PosRange), RandomRangef(-PosRange, PosRange), RandomRangef(0.0f, 10000.0f));
		ptr->rot = D3DXVECTOR3(RandomRangef(0.0f, 3.1415f), RandomRangef(0.0f, 3.1415f), RandomRangef(0.0f, 3.1415f));
		ptr->scale = 0.0f;
		ptr->rotValue = D3DXVECTOR3(RandomRangef(-RotRange, RotRange), RandomRangef(-RotRange, RotRange), RandomRangef(-RotRange, RotRange));
		ptr->rotValue *= 0.01f;
		ptr->moveSpeed = RandomRangef(-10.0f, -5.0f);
		ptr->hp = HARDCUBE_INIT_HP;
		ptr->id = i;
		ptr->active = false;

		ptr->collider.pos = &ptr->pos;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->collider.length = D3DXVECTOR3(HARDCUBE_SIZE*1.5f, HARDCUBE_SIZE*1.5f, HARDCUBE_SIZE*1.5f);

		CreateOFT(oft, (void*)ptr);
	}

	//初回のみの初期化
	if (!initialized)
	{
		//頂点バッファ作成
		pDevice->CreateVertexBuffer(sizeof(vtx), 0, 0, D3DPOOL_MANAGED, &vtxBuff, 0);
		pDevice->CreateVertexBuffer(sizeof(mtxWorld), 0, 0, D3DPOOL_MANAGED, &worldBuff, 0);
		CopyVtxBuff(sizeof(vtx), vtx, vtxBuff);
		CopyVtxBuff(sizeof(mtxWorld), mtxWorld, worldBuff);

		//頂点宣言作成
		D3DVERTEXELEMENT9 declareElems[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },		//頂点座標
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },		//UV座標
			{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },		//法線
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },		//ワールド変換行列
			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },		//ワールド変換行列
			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },		//ワールド変換行列
			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },		//ワールド変換行列
			D3DDECL_END()
		};
		pDevice->CreateVertexDeclaration(declareElems, &declare);

		//インデックスバッファ作成
		WORD index[6 * HARDCUBE_FIELD_NUM];
		for (int i = 0; i < 6; i++)
		{
			index[i * 6] = i * NUM_VERTEX;
			index[i * 6 + 1] = i * NUM_VERTEX + 1;
			index[i * 6 + 2] = i * NUM_VERTEX + 2;
			index[i * 6 + 3] = i * NUM_VERTEX + 2;
			index[i * 6 + 4] = i * NUM_VERTEX + 1;
			index[i * 6 + 5] = i * NUM_VERTEX + 3;
		}

		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void *p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		//シェーダー読み込み
		D3DXCreateEffectFromFile(pDevice, HARDCUBE_EFFECT_NAME, 0, 0, 0, 0, &effect, NULL);

		//テクスチャ読み込み
		for (int i = 0; i < 3; i++)
		{
			D3DXCreateTextureFromFile(pDevice, TextureName[i], &texture[i]);
		}

		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitHardCubeObject(int num)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++)
	{
		DisableHardCube(ptr);
	}

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(worldBuff);
		SAFE_RELEASE(effect);
		SAFE_RELEASE(declare);
		SAFE_RELEASE(indexBuff);

		for (int i = 0; i < HARDCUBE_TEX_NUM; i++) 
		{
			SAFE_RELEASE(texture[i]);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateHardCubeObject(void)
{
	CheckDestroyHardCube();
	OnUpdateHardCube();
	RotateHardCube();
	CalcHardCubeWorldMatrix();
	RegisterHardCubeToSpace();
}

/**************************************
描画処理
***************************************/
void DrawHardCubeObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DLIGHT9 light1, light2, light3;

	//ライト情報を取得してシェーダにセット
	pDevice->GetLight(0, &light1);
	effect->SetFloatArray("light1Dir", (float*)&light1.Direction, 3);
	effect->SetFloatArray("light1Diffuse", (float*)&light1.Diffuse, 4);
	effect->SetFloatArray("light1Ambient", (float*)&light1.Ambient, 4);

	pDevice->GetLight(0, &light2);
	effect->SetFloatArray("light2Dir", (float*)&light2.Direction, 3);
	effect->SetFloatArray("light2Diffuse", (float*)&light2.Diffuse, 4);
	effect->SetFloatArray("light2Ambient", (float*)&light2.Ambient, 4);

	pDevice->GetLight(0, &light3);
	effect->SetFloatArray("light3Dir", (float*)&light3.Direction, 3);
	effect->SetFloatArray("light3Diffuse", (float*)&light3.Diffuse, 4);
	effect->SetFloatArray("light3Ambient", (float*)&light3.Ambient, 4);

	//ストリーム周波数をセット
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (HARDCUBE_NUM_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点宣言、ストリームソース、インデックスバッファをセット
	pDevice->SetVertexDeclaration(declare);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(HARDCUBE_VTX));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetIndices(indexBuff);

	//ビュー行列、プロジェクション行列を設置
	effect->SetMatrix("mtxView", &GetBattleCameraView());
	effect->SetMatrix("mtxProj", &GetBattleCameraProjection());

	//テクスチャの数だけ描画
	for (int i = 0; i < HARDCUBE_TEX_NUM; i++)
	{
		if (i == 0)
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		else
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		effect->SetTexture("tex", texture[i]);
		effect->SetTechnique("tech");
		UINT passNum = 0;
		effect->Begin(&passNum, 0);
		effect->BeginPass(0);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, HARDCUBE_VTX_NUM, 0, HARDCUBE_FIELD_NUM * NUM_POLYGON);

		effect->EndPass();
		effect->End();
	}
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);

#if 0
	HARD_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++)
	{
		DrawBoundingCube(&ptr->collider);
	}
#endif
}

/**************************************
回転処理
***************************************/
void RotateHardCube(void)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++)
	{
		ptr->rot += ptr->rotValue;
	}
}

/**************************************
ワールド変換行列計算処理
***************************************/
void CalcHardCubeWorldMatrix(void)
{
	D3DXMATRIX *pWorld = &mtxWorld[0];
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
	HARD_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++, pWorld++)
	{
		D3DXMatrixIdentity(pWorld);

		D3DXMatrixScaling(&mtxScale, ptr->scale, ptr->scale, ptr->scale);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxScale);

		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->rot.y, ptr->rot.x, ptr->rot.z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxTranslate);
	}

	CopyVtxBuff(sizeof(mtxWorld), mtxWorld, worldBuff);
}


/**************************************
死亡判定
***************************************/
void CheckDestroyHardCube(void)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++)
	{
		if (!ptr->active)
			continue;

		if (ptr->hp <= 0.0f)
		{
			SetCubeExplosion(ptr->pos, PARTICLE_HARDCUBE_COLOR);
			PlaySE(SOUND_SMALLEXPL);
			//DamageAllCubeObject();

			if (ptr->type == HardCubeHomingType)
			{
				SetCameraShaker(BONUSCUBE_CAMERA_SHAKE_LENGTH);
				SetShockBlur(ptr->pos);
				PlaySE(SOUND_MIDDLEEXPL);
			}
			
			DisableHardCube(ptr);
		}
	}
}

/**************************************
衝突空間への登録処理
***************************************/
void RegisterHardCubeToSpace(void)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];

	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		RemoveObjectFromSpace(oft);

		if (ptr->active)
		{
			RegisterObjectToSpace(&ptr->collider, oft, OFT_HARDCUBE);
		}
	}
}

/**************************************
ロックオン処理
***************************************/
void LockonHardCubeObject(void)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
			continue;

		ptr = &cube[0];
		for (int j = 0; j < HARDCUBE_NUM_MAX; j++, ptr++)
		{
			if (!ptr->active)
				continue;

			if (CollisionTargetSite(i, &ptr->pos))
			{
				AddRockonTarget(i, &ptr->pos, &ptr->active, &ptr->hp);
			}

		}
	}
}

/**************************************
非アクティブ処理
***************************************/
void DisableHardCube(HARD_CUBE_OBJECT *ptr)
{
	ptr->active = false;
	ptr->scale = 0.0f;
	RemoveObjectFromSpace(&objectForTree[ptr->id]);
}

/**************************************
ハードキューブセット処理
***************************************/
bool SetHardCubeObject(D3DXVECTOR3 *setPos)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		if (ptr->active)
			continue;

		ptr->pos = *setPos;
		ptr->active = true;
		RegisterObjectToSpace(&ptr->collider, oft, OFT_HARDCUBE);
		ChangeStateHardCube(ptr, HardCubeInit);
		return true;
	}

	return false;
}

/**************************************
状態遷移処理
***************************************/
void ChangeStateHardCube(HARD_CUBE_OBJECT *ptr, int nextState)
{
	ptr->currentState = nextState;
	Enter[ptr->currentState](ptr);
}

/**************************************
各状態更新処理
***************************************/
void OnUpdateHardCube(void)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++)
	{
		if(ptr->active)
			Update[ptr->currentState](ptr);
	}
}

/**************************************
ハードキューブセット処理(fromステージデータ)
***************************************/
bool SetHardCubeObjectFromData(STAGE_DATA *data)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		if (ptr->active)
			continue;

		ptr->pos = data->initPos;
		ptr->goalPos = data->targetPos;
		ptr->type = data->type;
		ptr->controller1 = data->controller1;
		ptr->controller2 = data->controller2;
		ptr->active = true;
		RegisterObjectToSpace(&ptr->collider, oft, OFT_HARDCUBE);
		ChangeStateHardCube(ptr, HardCubeInit);
		return true;
	}

	return false;
}

/**************************************
ハードキューブセット処理(パラメータ直接指定)
***************************************/
HARD_CUBE_OBJECT* SetHardCubeObjectDirectData(D3DXVECTOR3 startPos, D3DXVECTOR3 goalPos, int type, D3DXVECTOR3 ctrl1, D3DXVECTOR3 ctrl2)
{
	HARD_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < HARDCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		if (ptr->active)
			continue;

		ptr->pos = startPos;
		ptr->goalPos = goalPos;
		ptr->type = type;
		ptr->controller1 = ctrl1;
		ptr->controller2 = ctrl2;
		ptr->active = true;
		RegisterObjectToSpace(&ptr->collider, oft, OFT_HARDCUBE);
		ChangeStateHardCube(ptr, HardCubeInit);
		return ptr;
	}

	return NULL;
}