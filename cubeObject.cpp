//=====================================
//
//キューブオブジェクト処理[cubeObject.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "cubeObject.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define CUBEOBJECT_EFFECT_NAME			"data/EFFECT/cubeObject.fx"
#define CUBEOBJECT_SIZE					(40.0f)
#define CUBEOBJECT_VTX_NUM				(24)
#define CUBEOBJECT_NUM_MAX				(1024)
#define CUBEOBJECT_FIELD_NUM			(6)

const char* texName[] = {
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
}CUBE_VTX;

/**************************************
グローバル変数
***************************************/
static CUBE_VTX vtx[CUBEOBJECT_VTX_NUM] = {
	//上
	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	//前
	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f },
	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f },
	//下
	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
	//後ろ
	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	//左
	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ -CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	{ -CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	//右
	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ CUBEOBJECT_SIZE,-CUBEOBJECT_SIZE, CUBEOBJECT_SIZE, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
};

static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff;
static LPDIRECT3DVERTEXDECLARATION9 declare;
static LPDIRECT3DTEXTURE9 texture[3];
static LPD3DXEFFECT effect;
static LPDIRECT3DINDEXBUFFER9 indexBuff;

static D3DXVECTOR3 pos[CUBEOBJECT_NUM_MAX], rot[CUBEOBJECT_VTX_NUM];
static D3DXMATRIX mtxWorld[CUBEOBJECT_NUM_MAX];

static D3DXVECTOR3 rotValue[CUBEOBJECT_NUM_MAX];
static float cubeSpeed[CUBEOBJECT_NUM_MAX];

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitCubeObject(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//キューブ生成範囲
	const float PosRange = 900.0f;
	const float RotRange = 5.0f;

	//パラメータ初期化
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++)
	{
		pos[i] = D3DXVECTOR3(RandomRange(-PosRange, PosRange), RandomRange(-PosRange, PosRange), RandomRange(0.0f, 20000.0f));
		rot[i] = D3DXVECTOR3(RandomRange(0.0f, 3.1415f), RandomRange(0.0f, 3.1415f), RandomRange(0.0f, 3.1415f));
		rotValue[i] = D3DXVECTOR3(RandomRange(-RotRange, RotRange), RandomRange(-RotRange, RotRange), RandomRange(-RotRange, RotRange));
		rotValue[i] *= 0.01f;
		cubeSpeed[i] = RandomRange(-50.0f, -5.0f);
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
	WORD index[6 * CUBEOBJECT_FIELD_NUM];
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
	D3DXCreateEffectFromFile(pDevice, CUBEOBJECT_EFFECT_NAME, 0, 0, 0, 0, &effect, NULL);

	//テクスチャ読み込み
	for (int i = 0; i < 3; i++)
	{
		D3DXCreateTextureFromFile(pDevice, texName[i], &texture[i]);
	}

}

/**************************************
終了処理
***************************************/
void UninitCubeObject(int num)
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

/**************************************
更新処理
***************************************/
void UpdateCubeObject(void)
{
	D3DXVECTOR3 *pPos = &pos[0];
	D3DXVECTOR3 *pRot = &rot[0];

	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, pPos++, pRot++)
	{
		pPos->z += cubeSpeed[i];
		if (pPos->z < 0.0f)
		{
			pPos->z += 20000.0f;
		}

		*pRot += rotValue[i];
	}

	pPos = &pos[0];
	pRot = &rot[0];
	D3DXMATRIX *pWorld = &mtxWorld[0];
	D3DXMATRIX mtxRot, mtxTranslate;
	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, pPos++, pRot++, pWorld++)
	{
		D3DXMatrixIdentity(pWorld);

		D3DXMatrixRotationYawPitchRoll(&mtxRot, pRot->y, pRot->x, pRot->z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, pPos->x, pPos->y, pPos->z);
		D3DXMatrixMultiply(pWorld, pWorld, &mtxTranslate);
	}

	CopyVtxBuff(sizeof(mtxWorld), mtxWorld, worldBuff);

}

/**************************************
描画処理
***************************************/
void DrawCubeObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DLIGHT9 light1, light2, light3;

	pDevice->GetLight(0, &light1);
	pDevice->GetLight(0, &light2);
	pDevice->GetLight(0, &light3);

	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (CUBEOBJECT_NUM_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	pDevice->SetVertexDeclaration(declare);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(CUBE_VTX));
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

	for (int i = 0; i < 3; i++)
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

}
