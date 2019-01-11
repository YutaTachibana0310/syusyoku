//=====================================
//
//ボーナスキューブ処理[bonusCube.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bonusCube.h"
#include "targetSite.h"
#include "particleManager.h"
#include "dataContainer.h"
#include "collisionManager.h"
#include "battleCamera.h"
#include "playerModel.h"

/**************************************
マクロ定義
***************************************/
#define BONUSCUBE_EFFECT_NAME			"data/EFFECT/cubeObject.fx"
#define BONUSCUBE_SIZE					(15.0f)
#define BONUSCUBE_NUM_MAX				(4)
#define BONUSCUBE_VTX_NUM				(24)
#define BONUSCUBE_FIELD_NUM				(6)
#define BONUSCUBE_TEX_NUM				(3)
#define BONUSCUBE_INIT_HP				(20.0f)

static const char* TextureName[BONUSCUBE_TEX_NUM] = {
	"data/TEXTURE/OBJECT/circuit09.png",
	"data/TEXTURE/OBJECT/circuit10.png",
	"data/TEXTURE/OBJECT/circuit11.png",
};

/**************************************
構造体定義
***************************************/
typedef struct {
	float posX, posY, posZ;
	float texU, texV;
	float norX, norY, norZ;
}BONUSCUBE_VTX;

/**************************************
グローバル変数
***************************************/
//単位頂点
static BONUSCUBE_VTX vtx[BONUSCUBE_VTX_NUM] = {
	//上
	{ -BONUSCUBE_SIZE, BONUSCUBE_SIZE, BONUSCUBE_SIZE, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
{ BONUSCUBE_SIZE, BONUSCUBE_SIZE, BONUSCUBE_SIZE, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
{ -BONUSCUBE_SIZE, BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
{ BONUSCUBE_SIZE, BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
//前
{ -BONUSCUBE_SIZE, BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
{ BONUSCUBE_SIZE, BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f },
{ -BONUSCUBE_SIZE,-BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f },
{ BONUSCUBE_SIZE,-BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f },
//下
{ -BONUSCUBE_SIZE,-BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
{ BONUSCUBE_SIZE,-BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
{ -BONUSCUBE_SIZE,-BONUSCUBE_SIZE, BONUSCUBE_SIZE, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
{ BONUSCUBE_SIZE,-BONUSCUBE_SIZE, BONUSCUBE_SIZE, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
//後ろ
{ BONUSCUBE_SIZE, BONUSCUBE_SIZE, BONUSCUBE_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
{ -BONUSCUBE_SIZE, BONUSCUBE_SIZE, BONUSCUBE_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
{ BONUSCUBE_SIZE,-BONUSCUBE_SIZE, BONUSCUBE_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
{ -BONUSCUBE_SIZE,-BONUSCUBE_SIZE, BONUSCUBE_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
//左
{ -BONUSCUBE_SIZE, BONUSCUBE_SIZE, BONUSCUBE_SIZE, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
{ -BONUSCUBE_SIZE, BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f },
{ -BONUSCUBE_SIZE,-BONUSCUBE_SIZE, BONUSCUBE_SIZE, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f },
{ -BONUSCUBE_SIZE,-BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
//右
{ BONUSCUBE_SIZE, BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
{ BONUSCUBE_SIZE, BONUSCUBE_SIZE, BONUSCUBE_SIZE, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
{ BONUSCUBE_SIZE,-BONUSCUBE_SIZE,-BONUSCUBE_SIZE, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
{ BONUSCUBE_SIZE,-BONUSCUBE_SIZE, BONUSCUBE_SIZE, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
};

static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff;			//頂点バッファ
static LPDIRECT3DVERTEXDECLARATION9 declare;				//頂点宣言
static LPDIRECT3DTEXTURE9 texture[BONUSCUBE_TEX_NUM];		//テクスチャ
static LPD3DXEFFECT effect;									//シェーダ
static LPDIRECT3DINDEXBUFFER9 indexBuff;					//インデックスバッファ

static D3DXMATRIX mtxWorld[BONUSCUBE_NUM_MAX];				//ワールド変換行列
static BONUS_CUBE_OBJECT cube[BONUSCUBE_NUM_MAX];			//ボーナスキューブ配列
static OBJECT_FOR_TREE objectForTree[BONUSCUBE_NUM_MAX];	//衝突判定用OBJECT_FOR_TREE配列


/**************************************
プロトタイプ宣言
***************************************/
void MoveBonusCube(void);				//移動処理
void RotateBonusCube(void);				//回転処理
void CalcBonusCubeWorldMatrix(void);	//ワールド変換行列計算処理
void CheckDestroyBonusCube(void);		//死亡判定	
void RegisterBonusCubeToSpace(void);	//衝突空間への登録処理

/**************************************
初期化処理
***************************************/
void InitBonusCube(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//キューブ生成範囲
	const float PosRange = 50.0f;
	const float RotRange = 5.0f;

	//パラメータ初期化
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		ptr->pos = D3DXVECTOR3(RandomRangef(-PosRange, PosRange), RandomRangef(-PosRange, PosRange), RandomRangef(0.0f, 10000.0f));
		ptr->rot = D3DXVECTOR3(RandomRangef(0.0f, 3.1415f), RandomRangef(0.0f, 3.1415f), RandomRangef(0.0f, 3.1415f));
		ptr->rotValue = D3DXVECTOR3(RandomRangef(-RotRange, RotRange), RandomRangef(-RotRange, RotRange), RandomRangef(-RotRange, RotRange));
		ptr->rotValue *= 0.01f;
		ptr->moveSpeed = RandomRangef(-10.0f, -5.0f);
		ptr->hp = BONUSCUBE_INIT_HP;
		ptr->active = false;

		ptr->collider.pos = &ptr->pos;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->collider.length = D3DXVECTOR3(BONUSCUBE_SIZE*1.5f, BONUSCUBE_SIZE*1.5f, BONUSCUBE_SIZE*1.5f);

		CreateOFT(oft, (void*)ptr);
	}
	cube[0].active = true;
	cube[0].pos.x = 0.0f;

	//初回のみの初期化
	if (num == 0)
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
		WORD index[6 * BONUSCUBE_FIELD_NUM];
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
		D3DXCreateEffectFromFile(pDevice, BONUSCUBE_EFFECT_NAME, 0, 0, 0, 0, &effect, NULL);

		//テクスチャ読み込み
		for (int i = 0; i < 3; i++)
		{
			D3DXCreateTextureFromFile(pDevice, TextureName[i], &texture[i]);
		}
	}
}

/**************************************
終了処理
***************************************/
void UninitBonusCube(int num)
{
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		ptr->active = false;
		RemoveObjectFromSpace(oft);
	}

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(worldBuff);
		SAFE_RELEASE(effect);
		SAFE_RELEASE(declare);
		SAFE_RELEASE(indexBuff);

		for (int i = 0; i < BONUSCUBE_TEX_NUM; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateBonusCube(void)
{
	CheckDestroyBonusCube();
	MoveBonusCube();
	RotateBonusCube();
	CalcBonusCubeWorldMatrix();
	RegisterBonusCubeToSpace();
}

/**************************************
描画処理
***************************************/
void DrawBonusCube(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DLIGHT9 light1, light2, light3;

	pDevice->GetLight(0, &light1);
	pDevice->GetLight(0, &light2);
	pDevice->GetLight(0, &light3);

	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (BONUSCUBE_NUM_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	pDevice->SetVertexDeclaration(declare);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(BONUSCUBE_VTX));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetIndices(indexBuff);

	effect->SetMatrix("mtxView", &GetBattleCameraView());
	effect->SetMatrix("mtxProj", &GetBattleCameraProjection());
	effect->SetFloatArray("light1Dir", (float*)&light1.Direction, 3);
	effect->SetFloatArray("light1Diffuse", (float*)&light1.Diffuse, 4);
	effect->SetFloatArray("light1Ambient", (float*)&light1.Ambient, 4);

	effect->SetFloatArray("light2Dir", (float*)&light2.Direction, 3);
	effect->SetFloatArray("light2Diffuse", (float*)&light2.Diffuse, 4);
	effect->SetFloatArray("light2Ambient", (float*)&light2.Ambient, 4);

	effect->SetFloatArray("light3Dir", (float*)&light3.Direction, 3);
	effect->SetFloatArray("light3Diffuse", (float*)&light3.Diffuse, 4);
	effect->SetFloatArray("light3Ambient", (float*)&light3.Ambient, 4);

	for (int i = 0; i < BONUSCUBE_TEX_NUM; i++)
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

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, BONUSCUBE_VTX_NUM, 0, BONUSCUBE_FIELD_NUM * NUM_POLYGON);

		effect->EndPass();
		effect->End();
	}
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);

#if 0
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++)
	{
		DrawBoundingCube(&ptr->collider);
	}
#endif
}

/**************************************
移動処理
***************************************/
void MoveBonusCube(void)
{
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++)
	{
		ptr->pos.z += ptr->moveSpeed;
		if (ptr->pos.z < 0.0f)
		{
			ptr->pos.z = 2500.0f;
		}
	}
}

/**************************************
回転処理
***************************************/
void RotateBonusCube(void)
{
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++)
	{
		ptr->rot += ptr->rotValue;
	}
}

/**************************************
ワールド変換行列計算処理
***************************************/
void CalcBonusCubeWorldMatrix(void)
{
	D3DXMATRIX *pWorld = &mtxWorld[0];
	D3DXMATRIX mtxRot, mtxTranslate;
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++, pWorld++)
	{
		D3DXMatrixIdentity(pWorld);

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
void CheckDestroyBonusCube(void)
{
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++)
	{
		if (ptr->hp <= 0.0f)
		{
			SetCubeExplosion(ptr->pos);
			//DamageAllCube();

			ptr->hp = BONUSCUBE_INIT_HP;
			ptr->pos.z = 4500.0f;
			ptr->active = true;
		}
	}
}

/**************************************
衝突空間への登録処理
***************************************/
void RegisterBonusCubeToSpace(void)
{
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];

	for (int i = 0; i < BONUSCUBE_NUM_MAX; i++, ptr++, oft++)
	{
		RemoveObjectFromSpace(oft);

		if (ptr->active)
		{
			RegisterObjectToSpace(&ptr->collider, oft, OFT_BONUSCUBE);
		}
	}
}

/**************************************
ロックオン処理
***************************************/
void LockonBonusCube(void)
{
	BONUS_CUBE_OBJECT *ptr = &cube[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
			continue;

		ptr = &cube[0];
		for (int j = 0; j < BONUSCUBE_NUM_MAX; j++, ptr++)
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