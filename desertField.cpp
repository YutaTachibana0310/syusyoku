//===============================================
//
//砂漠フィールド処理［desertField.cpp］
//Author:GP11A341 21 立花雄太
//===============================================
#include "desertField.h"
#include "fractal.h"
#include "input.h"
#include "MyLibrary.h"

/**********************************************
マクロ定義
***********************************************/
#define DESERTFIELD_TEXNAME		 "data/TEXTURE/desert1.jpg"
#define DESERTFIELD_BLOCK_NUM	(128)
#define DESERTFIELD_BLOCK_SIZE	(100.0f * 128.0f / DESERTFIELD_BLOCK_NUM)
#define DESERTFIELD_MAX			(3)
#define DESERTFIELD_INITPOS_Y	(-250.0f)
#define DESERTFIELD_SCROLLSPEED	(-50.0f)

/**********************************************
グローバル変数
**********************************************/
static LPDIRECT3DTEXTURE9 texture;
static LPDIRECT3DINDEXBUFFER9 idxBuff;
static D3DXMATRIX mtxWorld;

static int numVertex;
static int numIndex;
static int numPolygon;
static float sizeBlock;
static int numBlock;

static DESERTFIELD field[DESERTFIELD_MAX];

/************************************************
プロトタイプ宣言
***********************************************/
void SetDesertFieldVtxBuffer(LPDIRECT3DVERTEXBUFFER9 vtxBuff, float *heightMap);
void SetDesertFieldIndexBuffer(void);
void CreateDesertField(int n);

/*********************************************
初期化処理
**********************************************/
void InitDesertField(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	DESERTFIELD *ptr = &field[0];

	if (num == 0)
	{
		sizeBlock = DESERTFIELD_BLOCK_SIZE;
		numBlock = DESERTFIELD_BLOCK_NUM;
		numVertex = (numBlock + 1) * (numBlock + 1);
		numIndex = (numBlock + 1) * 2 * numBlock + (numBlock - 1) * 2;
		numPolygon = numBlock * numBlock * 2 + (numBlock - 1) * 4;

		// オブジェクトのインデックスバッファを生成
		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * numIndex,		// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
			D3DFMT_INDEX16,						// 使用するインデックスフォーマット
			D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
			&idxBuff,				// インデックスバッファインターフェースへのポインタ
			NULL)))								// NULLに設定
		{
			return;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			DESERTFIELD_TEXNAME,
			&texture);

		SetDesertFieldIndexBuffer();

		for (int i = 0; i < DESERTFIELD_MAX; i++, ptr++)
		{
			ptr->heightMap = (float*)malloc(sizeof(float) * numVertex);
			InitializeHeightMap2(field[i].heightMap, DESERTFIELD_BLOCK_NUM);

			//頂点バッファ生成
			if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
				D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
				FVF_VERTEX_3D,						// 使用する頂点フォーマット
				D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
				&ptr->vtxBuff,						// 頂点バッファインターフェースへのポインタ
				NULL)))								// NULLに設定
			{
				return;
			}
		}
	}

	ptr = &field[0];
	for (int i = 0; i < DESERTFIELD_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(0.0f, DESERTFIELD_INITPOS_Y, i * DESERTFIELD_BLOCK_SIZE * DESERTFIELD_BLOCK_NUM);
		CreateDesertField(i);
		//頂点バッファ設定
		SetDesertFieldVtxBuffer(ptr->vtxBuff, ptr->heightMap);
	}
}
/*********************************************
終了処理
**********************************************/
void UninitDesertField(void)
{
	SAFE_RELEASE(idxBuff);
	SAFE_RELEASE(texture);

	DESERTFIELD *ptr = &field[0];
	for (int i = 0; i < DESERTFIELD_MAX; i++)
	{
		SAFE_RELEASE(ptr->vtxBuff);
	}
}

/*********************************************
更新処理
**********************************************/
void UpdateDesertField(void)
{
	DESERTFIELD *ptr = &field[0];
	for (int i = 0; i < DESERTFIELD_MAX; i++, ptr++)
	{
		ptr->pos.z += DESERTFIELD_SCROLLSPEED;

		if (ptr->pos.z < -(numBlock / 2.0f) * sizeBlock)
		{
			ptr->pos.z += (DESERTFIELD_MAX * numBlock) * sizeBlock;
			InitializeHeightMap2(ptr->heightMap, DESERTFIELD_BLOCK_NUM);
			CreateDesertField(i);
		}
	}

	if (GetKeyboardRepeat(DIK_F5))
	{
		InitDesertField(1);

	}
}

/*********************************************
描画処理
**********************************************/
void DrawDesertField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, texture);

	DESERTFIELD *ptr = &field[0];
	for (int i = 0; i < DESERTFIELD_MAX; i++, ptr++)
	{
		D3DXMatrixIdentity(&mtxWorld);

		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetStreamSource(0, ptr->vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->SetIndices(idxBuff);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVertex, 0, numPolygon);
	}
}
/*********************************************
頂点バッファ設定処理
**********************************************/
void SetDesertFieldVtxBuffer(LPDIRECT3DVERTEXBUFFER9 vtxBuff, float *heightMap)
{
	VERTEX_3D*pVtx;

	const float texSizeX = 0.5f;
	const float texSizeZ = 0.5f;

	//頂点データの範囲をロック
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報設定
	for (int z = 0; z < (numBlock + 1); z++)
	{
		for (int x = 0; x < (numBlock + 1); x++)
		{
			pVtx[z * (numBlock + 1) + x].vtx.x = -(numBlock / 2.0f) * sizeBlock + x * sizeBlock;
			pVtx[z * (numBlock + 1) + x].vtx.y = heightMap[z * (numBlock + 1) + x];
			//pVtx[z * (numBlock + 1) + x].vtx.y = map[z * (numBlock + 1) + x];
			pVtx[z * (numBlock + 1) + x].vtx.z = (numBlock / 2.0f) * sizeBlock - z * sizeBlock;

			pVtx[z * (numBlock + 1) + x].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[z * (numBlock + 1) + x].tex.x = texSizeX * x;
			pVtx[z * (numBlock + 1) + x].tex.y = texSizeZ * z;
		}
	}

	//法線設定
	for (int z = 0; z < (numBlock + 1); z++)
	{
		for (int x = 0; x < (numBlock + 1); x++)
		{
			if (x == numBlock || z == numBlock)
			{
				pVtx[z * (numBlock + 1) + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				D3DXVECTOR3 vec1 = pVtx[z * (numBlock + 1) + x + 1].vtx - pVtx[z * (numBlock + 1) + x].vtx;
				D3DXVECTOR3 vec2 = pVtx[(z + 1) * (numBlock + 1) + x].vtx - pVtx[z * (numBlock + 1) + x].vtx;
				D3DXVECTOR3 normal;
				D3DXVec3Cross(&pVtx[z * (numBlock + 1) + x].nor, &vec2, &vec1);
				D3DXVec3Normalize(&pVtx[z * (numBlock + 1) + x].nor, &pVtx[z * (numBlock + 1) + x].nor);
			}
		}
	}

	//アンロック
	vtxBuff->Unlock();
}

/***********************************************************
インデックスバッファ設定処理
***********************************************************/
void SetDesertFieldIndexBuffer(void)
{
	WORD *pIdx;

	//ロック
	idxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int cntIndex = 0;
	for (int z = 0; z < numBlock; z++)
	{
		//縮退ポリゴン用設定
		if (z > 0)
		{
			pIdx[cntIndex] = (z + 1) * (numBlock + 1);
			cntIndex++;
		}

		for (int x = 0; x < (numBlock + 1); x++)
		{
			pIdx[cntIndex] = (z + 1) * (numBlock + 1) + x;
			cntIndex++;
			pIdx[cntIndex] = z * (numBlock + 1) + x;
			cntIndex++;
		}

		//縮退ポリゴン用設定
		if (z < (numBlock - 1))
		{
			pIdx[cntIndex] = z * (numBlock + 1) + numBlock;
			cntIndex++;
		}
	}

	idxBuff->Unlock();
}

void CreateDesertField(int n)
{
	int prev = WrapAround(0, DESERTFIELD_MAX, n - 1);
	float topAvr = 0.0f;

	if (n < DESERTFIELD_MAX)
	{
		for (int x = 0; x < (numBlock + 1); x++)
		{
			topAvr = (field[n].heightMap[numBlock * (numBlock + 1) + x] + field[prev].heightMap[x]) / 2.0f;
			field[n].heightMap[numBlock * (numBlock + 1) + x] = field[prev].heightMap[x] = topAvr;
		}
		SetDesertFieldVtxBuffer(field[n].vtxBuff, field[n].heightMap);
		SetDesertFieldVtxBuffer(field[prev].vtxBuff, field[prev].heightMap);
	}
}