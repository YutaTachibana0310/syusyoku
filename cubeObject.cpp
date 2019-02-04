//=====================================
//
//キューブオブジェクト処理[cubeObject.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "cubeObject.h"
#include "targetSite.h"
#include "playerModel.h"
#include "particleManager.h"
#include "dataContainer.h"
#include "collisionManager.h"
#include "debugWindow.h"
#include "soundEffectManager.h"

/**************************************
マクロ定義
***************************************/
#define CUBEOBJECT_EFFECT_NAME			"data/EFFECT/cubeObject.fx"
#define CUBEOBJECT_SIZE					(12.0f)		//キューブオブジェクトのサイズ
#define CUBEOBJECT_VTX_NUM				(24)		//キューブオブジェクトの頂点数
#define CUBEOBJECT_FIELD_NUM			(6)			//キューブオブジェクトの面数
#define CUBEOBJECT_TEX_NUM				(3)			//テクスチャ枚数
#define CUBEOBJECT_INIT_HP				(0.01f)		//初期HP
#define CUBEOBJECT_ADD_SCORE			(100)		//破壊時の加算スコア
#define CUBEOBJECT_SCALE_MAX			(1.5f)		//キューブオブジェクト最大スケール
#define CUBEOBJECT_SCALE_MIN			(0.7f)		//キューブオブジェクト最小スケール
#define CUBEOBJECT_SPEED_MAX			(10.0f)		//キューブオブジェクト最大スピード
#define CUBEOBJECT_SPEED_MIN			(5.0f)		//キューブオブジェクト最小スピード

#define CUBEOBJECT_SPEED_RANGE			(3.0f)
#define PARTICLE_CUBE_COLOR				(D3DCOLOR_RGBA(103, 147, 255, 255))

//テクスチャ名
static const char* texName[CUBEOBJECT_TEX_NUM] = {
	"data/TEXTURE/OBJECT/circuit00.jpg",
	"data/TEXTURE/OBJECT/circuit04.png",
	"data/TEXTURE/OBJECT/circuit05.png",
};

/**************************************
構造体定義
***************************************/
typedef struct
{
	float posX, posY, posZ;
	float texU, texV;
	float norX, norY, norZ;
}CUBE_VTX;	//単位頂点構造体

/**************************************
グローバル変数
***************************************/
//キューブオブジェクトの単位頂点
//static CUBE_VTX vtx[CUBEOBJECT_VTX_NUM] = {
//	//上
//	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
//	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
//	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
//	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
//	//前
//	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
//	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f },
//	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f },
//	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f },
//	//下
//	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
//	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
//	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
//	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
//	//後ろ
//	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
//	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
//	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
//	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
//	//左
//	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
//	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f },
//	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f },
//	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
//	//右
//	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
//	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
//	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
//	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
//};

static CUBE_VTX vtx[24] = {
	{ 0.0f, 2 * CUBEOBJECT_SIZE,0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f} ,
	{ CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
	{ -CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},

	{ 0.0f, 2 * CUBEOBJECT_SIZE,0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{ CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},

	{ 0.0f, 2 * CUBEOBJECT_SIZE,0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{-CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	{CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

	{ 0.0f, 2 * CUBEOBJECT_SIZE,0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{ -CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	{-CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

	{ 0.0f, -2 * CUBEOBJECT_SIZE, 0.0f,0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{ -CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },

	{ 0.0f, -2 * CUBEOBJECT_SIZE,0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{ CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },

	{ 0.0f, -2 * CUBEOBJECT_SIZE, 0.0f,0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{ CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
	{ -CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },

	{ 0.0f, -2 * CUBEOBJECT_SIZE, 0.0f,0.5f, 0.5f, 0.0f, 0.0f, 0.0f } ,
	{ -CUBEOBJECT_SIZE, 0.0f, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ -CUBEOBJECT_SIZE, 0.0f, -CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
};

static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff;		//頂点バッファ
static LPDIRECT3DVERTEXDECLARATION9 declare;			//頂点宣言
static LPDIRECT3DTEXTURE9 texture[3];					//テクスチャ
static LPD3DXEFFECT effect;								//シェーダ
static LPDIRECT3DINDEXBUFFER9 indexBuff;				//インデックスバッファ

static D3DXMATRIX mtxWorld[CUBEOBJECT_NUM_MAX];			//ワールド変換行列

static CUBE_OBJECT cube[CUBEOBJECT_NUM_MAX];			//キューブオブジェクト配列
static OBJECT_FOR_TREE objectForTree[CUBEOBJECT_NUM_MAX]; //衝突判定用OBJECT_FOR_TREE配列

static int cntCube;
/**************************************
プロトタイプ宣言
***************************************/
void MoveCubeObject(void);					//移動処理
void RotationCubeObject(void);				//回転処理
void CalcCubeObjectWorldMartix(void);		//ワールド変換行列計算処理
void CheckDestroyCubeObject(void);			//死亡判定
void RegisterCubeObjectToSpace(void);		//衝突空間への登録処理
void DisableCubeObject(CUBE_OBJECT *ptr);	//非アクティブ処理

/**************************************
初期化処理
***************************************/
void InitCubeObject(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//キューブ生成範囲
	const float PosRange = 150.0f;
	const float RotRange = 5.0f;

	//パラメータ初期化
	CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++, oft++)
	{
		ptr->pos = D3DXVECTOR3(RandomRangef(-PosRange, PosRange), RandomRangef(-PosRange, PosRange), RandomRangef(0.0f, 20000.0f));
		ptr->rot = D3DXVECTOR3(RandomRangef(0.0f, 3.1415f), RandomRangef(0.0f, 3.1415f), RandomRangef(0.0f, 3.1415f));
		ptr->scale = 0.0f;

		ptr->rotValue = D3DXVECTOR3(RandomRangef(-RotRange, RotRange), RandomRangef(-RotRange, RotRange), RandomRangef(-RotRange, RotRange));
		ptr->rotValue *= 0.01f;
		ptr->hp = CUBEOBJECT_INIT_HP;
		ptr->id = i;
		ptr->active = false;

		ptr->collider.pos = &ptr->pos;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->collider.length = D3DXVECTOR3(CUBEOBJECT_SIZE*1.2f, CUBEOBJECT_SIZE*1.2f, CUBEOBJECT_SIZE*1.2f);

		CreateOFT(oft, (void*)ptr);
	}

	//初回のみの初期化
	if (!initialized)
	{

		//法線計算
		for (int i = 0; i < 8; i++)
		{
			D3DXVECTOR3 vec1, vec2;
			vec1 = D3DXVECTOR3(vtx[i * 3 + 2].posX, vtx[i * 3 + 2].posY, vtx[i * 3 + 2].posZ) - D3DXVECTOR3(vtx[i * 3].posX, vtx[i * 3].posY, vtx[i * 3].posZ);
			vec2 = D3DXVECTOR3(vtx[i * 3 + 1].posX, vtx[i * 3 + 1].posY, vtx[i * 3 + 1].posZ) - D3DXVECTOR3(vtx[i * 3].posX, vtx[i * 3].posY, vtx[i * 3].posZ);
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &vec2, &vec1);
			D3DXVec3Normalize(&normal, &normal);
			for (int j = 0; j < 3; j++)
			{
				vtx[i * 3 + j].norX = normal.x;
				vtx[i * 3 + j].norY = normal.y;
				vtx[i * 3 + j].norZ = normal.z;
			}
		}

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
		/*WORD index[6 * CUBEOBJECT_FIELD_NUM];
		for (int i = 0; i < 6; i++)
		{
			index[i * 6] = i * NUM_VERTEX;
			index[i * 6 + 1] = i * NUM_VERTEX + 1;
			index[i * 6 + 2] = i * NUM_VERTEX + 2;
			index[i * 6 + 3] = i * NUM_VERTEX + 2;
			index[i * 6 + 4] = i * NUM_VERTEX + 1;
			index[i * 6 + 5] = i * NUM_VERTEX + 3;
		}*/

		WORD index[24];
		for (int i = 0; i < 24; i++)
		{
			index[i] = i;
		}

		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, 0);
		void *p = NULL;
		indexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		indexBuff->Unlock();

		//シェーダー読み込み
		D3DXCreateEffectFromFile(pDevice, CUBEOBJECT_EFFECT_NAME, 0, 0, 0, 0, &effect, NULL);

		//テクスチャ読み込み
		for (int i = 0; i < 3; i++)
		{
			D3DXCreateTextureFromFile(pDevice, texName[i], &texture[i]);
		}

		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitCubeObject(int num)
{
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++, oft++)
	{
		DisableCubeObject(ptr);
	}

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(worldBuff);
		SAFE_RELEASE(effect);
		SAFE_RELEASE(declare);
		SAFE_RELEASE(indexBuff);

		for (int i = 0; i < 3; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateCubeObject(void)
{
	CheckDestroyCubeObject();

	MoveCubeObject();

	RotationCubeObject();

	CalcCubeObjectWorldMartix();

	RegisterCubeObjectToSpace();
}

/**************************************
描画処理
***************************************/
void DrawCubeObject(void)
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

	//ストリーム周波数設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (CUBEOBJECT_NUM_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点宣言とストリームソース、インデックスバッファを設定
	pDevice->SetVertexDeclaration(declare);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(CUBE_VTX));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetIndices(indexBuff);

	//ビュー行列、プロジェクション行列を設定
	effect->SetMatrix("mtxView", &GetBattleCameraView());
	effect->SetMatrix("mtxProj", &GetBattleCameraProjection());

	//テクスチャの枚数だけ描画
	for (int i = 0; i < CUBEOBJECT_TEX_NUM; i++)
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

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, CUBEOBJECT_VTX_NUM, 0, CUBEOBJECT_FIELD_NUM * NUM_POLYGON);

		effect->EndPass();
		effect->End();
	}
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);

#if 0
	CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++)
	{
		DrawBoundingCube(&ptr->collider);
	}
#endif

}

/**************************************
移動処理
***************************************/
void MoveCubeObject(void)
{
	CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++)
	{
		if (!ptr->active)
			continue;

		ptr->pos.z += ptr->moveSpeed;
		if (ptr->pos.z < -1000.0f)
		{
			DisableCubeObject(ptr);
		}
	}
}

/**************************************
回転処理
***************************************/
void RotationCubeObject(void)
{
	CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++)
	{
		ptr->rot += ptr->rotValue;
	}
}

/**************************************
ワールド変換行列計算処理
***************************************/
void CalcCubeObjectWorldMartix(void)
{
	D3DXMATRIX *pWorld = &mtxWorld[0];
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
	CUBE_OBJECT *ptr = &cube[0];

	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, pWorld++, ptr++)
	{
		D3DXMatrixIdentity(pWorld);

		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->rot.y, ptr->rot.x, ptr->rot.z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxRot);

		D3DXMatrixScaling(&mtxScale, ptr->scale, ptr->scale, ptr->scale);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxScale);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxTranslate);
	}

	CopyVtxBuff(sizeof(mtxWorld), mtxWorld, worldBuff);
}

/**************************************
死亡判定
***************************************/
void CheckDestroyCubeObject(void)
{
	CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++)
	{
		if (!ptr->active)
			continue;

		if (ptr->hp <= 0.0f)
		{
			SetCubeExplosion(ptr->pos, PARTICLE_CUBE_COLOR);
			AddScore(CUBEOBJECT_ADD_SCORE);
			PlaySE_3D(SOUND_SMALLEXPL, ptr->pos.z);
			DisableCubeObject(ptr);
		}
	}
}

/*****************************************
キューブロックオン判定
******************************************/
void LockonCubeObject(void)
{
	CUBE_OBJECT *ptr = &cube[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
		{
			continue;
		}

		ptr = &cube[0];
		for (int j = 0; j < CUBEOBJECT_NUM_MAX; j++, ptr++)
		{
			if (!ptr->active)
			{
				continue;
			}

			if (CollisionTargetSite(i, &ptr->pos))
			{
				AddRockonTarget(i, &ptr->pos, &ptr->active, &ptr->hp);
			}
		}
	}
}

/*****************************************
衝突判定空間への登録処理
******************************************/
void RegisterCubeObjectToSpace(void)
{
	CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];

	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++, oft++)
	{
		RemoveObjectFromSpace(oft);

		if (ptr->active)
		{
			RegisterObjectToSpace(&ptr->collider, oft, OFT_CUBEOBJECT);
		}
	}
}

/*****************************************
全キューブへのダメージ処理
******************************************/
void DamageAllCubeObject(void)
{
	CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++)
	{
		if (!ptr->active)
			continue;

		ptr->hp -= 1.0f;
	}
}

/*****************************************
非アクティブ処理
******************************************/
void DisableCubeObject(CUBE_OBJECT *ptr)
{
	ptr->active = false;
	ptr->scale = 0.0f;
	cntCube--;
	RemoveObjectFromSpace(&objectForTree[ptr->id]);
}

/*****************************************
セット処理
******************************************/
bool SetCubeObject(D3DXVECTOR3 *setPos, float speed = 8.0f)
{
	CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, ptr++, oft++)
	{
		if (ptr->active)
			continue;

		ptr->pos = *setPos;
		ptr->scale = RandomRangef(CUBEOBJECT_SCALE_MIN, CUBEOBJECT_SCALE_MAX);
		ptr->moveSpeed = -RandomRangef(CUBEOBJECT_SPEED_MIN, CUBEOBJECT_SPEED_MAX);
		ptr->hp = CUBEOBJECT_INIT_HP;
		ptr->collider.length = D3DXVECTOR3(CUBEOBJECT_SIZE * ptr->scale, CUBEOBJECT_SIZE * ptr->scale, CUBEOBJECT_SIZE * ptr->scale);
		ptr->moveSpeed = -RandomRangef(speed - CUBEOBJECT_SPEED_RANGE, speed + CUBEOBJECT_SPEED_RANGE);
		ptr->active = true;
		RegisterObjectToSpace(&ptr->collider, oft, OFT_CUBEOBJECT);
		cntCube++;
		return true;
	}

	return false;
}

/*****************************************
アドレス取得処理
******************************************/
CUBE_OBJECT *GetCubeObjectAdr(int num)
{
	return &cube[num];
}

/*****************************************
デバッグ情報表示
******************************************/
void DrawDebugWindowCube(void)
{
	BeginDebugWindow("CubeObject");

	DebugText("Active : %d", cntCube);

	EndDebugWindow("CubeObject");
}