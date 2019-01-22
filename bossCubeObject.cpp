//=====================================
//
//テンプレート処理[bossCubeObject.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "bossCubeObject.h"
#include "collisionManager.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define BOSSCUBE_EFFECT_NAME	"data/EFFECT/cubeObject.fx"
#define BOSSCUBE_VTX_NUM		(24)
#define BOSSCUBE_SIZE			(40.0f)
#define BOSSCUBE_NUM_MAX		(28)
#define BOSSCUBE_FIELD_NUM		(6)
#define BOSSCUBE_TEX_NUM		(3)

static const char* TexName[BOSSCUBE_TEX_NUM] = {
	"data/TEXTURE/OBJECT/circuit00.jpg",
	"data/TEXTURE/OBJECT/circuit04.png",
	"data/TEXTURE/OBJECT/circuit05.png",
};

/**************************************
構造体定義
***************************************/
typedef struct {
	float posX, posY, posZ;
	float texU, texV;
	float norX, norY, norZ;
}BOSS_VTX;

/**************************************
グローバル変数
***************************************/
static BOSS_VTX vtx[BOSSCUBE_VTX_NUM] = {
	//上
	{ -BOSSCUBE_SIZE, BOSSCUBE_SIZE, BOSSCUBE_SIZE, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ BOSSCUBE_SIZE, BOSSCUBE_SIZE, BOSSCUBE_SIZE, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{ -BOSSCUBE_SIZE, BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	{ BOSSCUBE_SIZE, BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
	//前
	{ -BOSSCUBE_SIZE, BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	{ BOSSCUBE_SIZE, BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f },
	{ -BOSSCUBE_SIZE,-BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f },
	{ BOSSCUBE_SIZE,-BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f },
	//下
	{ -BOSSCUBE_SIZE,-BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f },
	{ BOSSCUBE_SIZE,-BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f },
	{ -BOSSCUBE_SIZE,-BOSSCUBE_SIZE, BOSSCUBE_SIZE, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f },
	{ BOSSCUBE_SIZE,-BOSSCUBE_SIZE, BOSSCUBE_SIZE, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
	//後ろ
	{ BOSSCUBE_SIZE, BOSSCUBE_SIZE, BOSSCUBE_SIZE, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ -BOSSCUBE_SIZE, BOSSCUBE_SIZE, BOSSCUBE_SIZE, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
	{ BOSSCUBE_SIZE,-BOSSCUBE_SIZE, BOSSCUBE_SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	{ -BOSSCUBE_SIZE,-BOSSCUBE_SIZE, BOSSCUBE_SIZE, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	//左
	{ -BOSSCUBE_SIZE, BOSSCUBE_SIZE, BOSSCUBE_SIZE, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ -BOSSCUBE_SIZE, BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f },
	{ -BOSSCUBE_SIZE,-BOSSCUBE_SIZE, BOSSCUBE_SIZE, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	{ -BOSSCUBE_SIZE,-BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	//右
	{ BOSSCUBE_SIZE, BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ BOSSCUBE_SIZE, BOSSCUBE_SIZE, BOSSCUBE_SIZE, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ BOSSCUBE_SIZE,-BOSSCUBE_SIZE,-BOSSCUBE_SIZE, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f },
	{ BOSSCUBE_SIZE,-BOSSCUBE_SIZE, BOSSCUBE_SIZE, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
};

static LPDIRECT3DVERTEXBUFFER9 vtxBuff, worldBuff;		//頂点バッファ
static LPDIRECT3DVERTEXDECLARATION9 declare;			//頂点宣言
static LPDIRECT3DTEXTURE9 texture[3];					//テクスチャ
static LPD3DXEFFECT effect;								//シェーダ
static LPDIRECT3DINDEXBUFFER9 indexBuff;				//インデックスバッファ

static D3DXMATRIX mtxWorld[BOSSCUBE_NUM_MAX];			//ワールド変換行列

static BOSS_CUBE_OBJECT cube[BOSSCUBE_NUM_MAX];			//キューブオブジェクト配列
static OBJECT_FOR_TREE objectForTree[BOSSCUBE_NUM_MAX]; //衝突判定用OBJECT_FOR_TREE配列

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitBossCubeObject(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//パラメータ初期化
	BOSS_CUBE_OBJECT *ptr = &cube[0];
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	
	//初回のみの初期化
	if (num == 0)
	{
		//頂点バッファ作成
		pDevice->CreateVertexBuffer(sizeof(vtx), 0, 0, D3DPOOL_MANAGED, &vtxBuff, 0);
		pDevice->CreateVertexBuffer(sizeof(mtxWorld), 0, 0, D3DPOOL_MANAGED, &worldBuff, 0);
		CopyVtxBuff(sizeof(vtx), vtx, vtxBuff);
		CopyVtxBuff(sizeof(mtxWorld), mtxWorld, worldBuff);

		//頂点宣言作成
		D3DVERTEXELEMENT9 declareElems[] = {
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

		//インデックバッファ作成
		WORD index[6 * BOSSCUBE_FIELD_NUM];
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
		D3DXCreateEffectFromFile(pDevice, BOSSCUBE_EFFECT_NAME, 0, 0, 0, 0, &effect, NULL);

		//テクスチャ読み込み
		for (int i = 0; i < BOSSCUBE_TEX_NUM; i++)
		{
			D3DXCreateTextureFromFile(pDevice, TexName[i], &texture[i]);
		}
	}
}

/**************************************
終了処理
***************************************/
void UninitBossCubeObject(int num)
{
	OBJECT_FOR_TREE *oft = &objectForTree[0];
	BOSS_CUBE_OBJECT *ptr = &cube[0];
	for (int i = 0; i < BOSSCUBE_NUM_MAX; i++, ptr++, oft++)
	{

	}

	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(worldBuff);
		SAFE_RELEASE(effect);
		SAFE_RELEASE(declare);
		SAFE_RELEASE(indexBuff);

		for (int i = 0; i < BOSSCUBE_TEX_NUM; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateBossCubeObject(void)
{

}

/**************************************
描画処理
***************************************/
void DrawBossCubeObject(void)
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
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | (BOSSCUBE_NUM_MAX));
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点宣言とストリームソース、インデックスバッファを設定
	pDevice->SetVertexDeclaration(declare);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(BOSS_VTX));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetIndices(indexBuff);

	//ビュー行列、プロジェクション行列を設定
	effect->SetMatrix("mtxView", &GetBattleCameraView());
	effect->SetMatrix("mtxProj", &GetBattleCameraProjection());

	//テクスチャの枚数だけ描画
	for (int i = 0; i < BOSSCUBE_TEX_NUM; i++)
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

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, BOSSCUBE_VTX_NUM, 0, BOSSCUBE_FIELD_NUM * NUM_POLYGON);

		effect->EndPass();
		effect->End();
	}
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
}
