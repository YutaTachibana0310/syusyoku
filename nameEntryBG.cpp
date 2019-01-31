//=====================================
//
//ネームエントリーBG処理[nameEntryBG.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryBG.h"
#include "fractal.h"
#include "debugWindow.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define NAMEENTRYBG_TEXTURE_NAME		"data/TEXTURE/OBJECT/nameEntryBG.jpg"
#define NAMEENTRYBG_VTX_NUM				(128)
#define NAMEENTRYBG_BLOCK_SIZE			(150.0f)
#define NAMEENTRYBG_FADE_DURATION		(100)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static LPDIRECT3DINDEXBUFFER9 indexBuff;

static int numBlock;
static int numVertex;
static int numPolygon;
static int numIndex;
static float sizeX, sizeZ;
static float heightMagni = 1.0f;
static float heightMap[NAMEENTRYBG_VTX_NUM*NAMEENTRYBG_VTX_NUM];
static D3DXVECTOR3 rot, pos;
static int cntFrame;
static bool flgInclement;

/**************************************
プロトタイプ宣言
***************************************/
void SetVertexBuffer(void);
void SetIndexBuffer(void);
void SetDiffuseNameEntryBG(float alpha);

/**************************************
初期化処理
***************************************/
void InitNameEntryBG(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		texture = CreateTextureFromFile((LPSTR)NAMEENTRYBG_TEXTURE_NAME, pDevice);

		//各パラメータ計算
		numBlock = NAMEENTRYBG_VTX_NUM - 1;
		numVertex = NAMEENTRYBG_VTX_NUM * NAMEENTRYBG_VTX_NUM;
		numIndex = (numBlock + 1) * 2 * numBlock + (numBlock - 1) * 2;
		numPolygon = numBlock * numBlock * 2 + (numBlock - 1) * 4;

		sizeX = sizeZ = NAMEENTRYBG_BLOCK_SIZE;

		//頂点バッファ作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&vtxBuff,
			NULL);

		//インデックスバッファ作成
		pDevice->CreateIndexBuffer(sizeof(WORD) * numIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&indexBuff,
			NULL);

		SetUpFractal(NAMEENTRYBG_VTX_NUM, 2.5f, 4000.0f, 0.95f);

	}

	//中点変位法により高さマップを生成
	InitializeHeightMap2(heightMap, NAMEENTRYBG_VTX_NUM);

	//頂点バッファ、インデックスバッファを設定
	SetVertexBuffer();
	SetIndexBuffer();
	initialized = true;

	flgInclement = true;
	cntFrame = 0;
}

/**************************************
終了処理
***************************************/
void UninitNameEntryBG(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(indexBuff);
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateNameEntryBG(void)
{
	rot.y += 0.001f;

	cntFrame += flgInclement ? 1 : -1;

	if (cntFrame == 0 || cntFrame == NAMEENTRYBG_FADE_DURATION)
		flgInclement = !flgInclement;
}

/**************************************
描画処理
***************************************/
void DrawNameEntryBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxWorld, mtxRot, mtxTranslate;

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(indexBuff);

	pDevice->SetTexture(0, texture);

	SetDiffuseNameEntryBG(1.0f);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVertex, 0, numPolygon);

	float alpha = EaseInOutCubic((float)cntFrame / (float)NAMEENTRYBG_FADE_DURATION, 0.0f, 1.0f);
	SetDiffuseNameEntryBG(alpha);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVertex, 0, numPolygon);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

/**************************************
頂点バッファ設定処理
***************************************/
void SetVertexBuffer(void)
{
	VERTEX_3D *pVtx;

	const float texSize = 0.1f;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int z = 0; z < NAMEENTRYBG_VTX_NUM; z++)
	{
		for (int x = 0; x < NAMEENTRYBG_VTX_NUM; x++)
		{
			pVtx[x + z * NAMEENTRYBG_VTX_NUM].vtx.x = -(numBlock / 2.0f) * sizeX + x * sizeX;

			pVtx[x + z * NAMEENTRYBG_VTX_NUM].vtx.y = heightMap[x + z * NAMEENTRYBG_VTX_NUM] * heightMagni;

			pVtx[x + z * NAMEENTRYBG_VTX_NUM].vtx.z = (numBlock / 2.0f) * sizeZ - z * sizeZ;

			pVtx[x + z * NAMEENTRYBG_VTX_NUM].diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

			pVtx[x + z * NAMEENTRYBG_VTX_NUM].tex.x = texSize * x;
			pVtx[x + z * NAMEENTRYBG_VTX_NUM].tex.y = texSize * z;
		}
	}

	for (int z = 0; z < NAMEENTRYBG_VTX_NUM; z++)
	{
		for (int x = 0; x < NAMEENTRYBG_VTX_NUM; x++)
		{
			if (z == numBlock || x == numBlock)
			{
				pVtx[x + z * NAMEENTRYBG_VTX_NUM].nor = D3DXVECTOR3(0.0, 1.0f, 0.0);
			}
			else
			{
				D3DXVECTOR3 vec1 = pVtx[x + z * NAMEENTRYBG_VTX_NUM + 1].vtx - pVtx[x + z * NAMEENTRYBG_VTX_NUM].vtx;
				D3DXVECTOR3 vec2 = pVtx[x + (z + 1) * NAMEENTRYBG_VTX_NUM + 1].vtx - pVtx[x + z * NAMEENTRYBG_VTX_NUM].vtx;
				D3DXVECTOR3 normal;
				D3DXVec3Cross(&normal, &vec2, &vec1);
				D3DXVec3Normalize(&pVtx[x + z * NAMEENTRYBG_VTX_NUM].nor, &normal);
			}
		}
	}

	float maxHeight = 0.0f;
	for (int i = 0; i < NAMEENTRYBG_VTX_NUM*NAMEENTRYBG_VTX_NUM; i++)
	{
		if (pVtx[i].vtx.y > maxHeight)
		{
			maxHeight = pVtx[i].vtx.y;
		}
	}
	pos.y = -maxHeight;

	vtxBuff->Unlock();
}

/**************************************
インデックスバッファ設定処理
***************************************/
void SetIndexBuffer(void)
{
	WORD *pIdx;

	indexBuff->Lock(0, 0, (void**)&pIdx, 0);

	int cntIndex = 0;
	for (int z = 0; z < numBlock; z++)
	{
		if (z > 0)
		{
			pIdx[cntIndex] = (z + 1) * NAMEENTRYBG_VTX_NUM;
			cntIndex++;
		}

		for (int x = 0; x < NAMEENTRYBG_VTX_NUM; x++)
		{
			pIdx[cntIndex] = (z + 1) * NAMEENTRYBG_VTX_NUM + x;
			cntIndex++;
			pIdx[cntIndex] = z * NAMEENTRYBG_VTX_NUM + x;
			cntIndex++;
		}

		if (z < (numBlock - 1))
		{
			pIdx[cntIndex] = z * NAMEENTRYBG_VTX_NUM + numBlock;
			cntIndex++;
		}
	}

	indexBuff->Unlock();
}

/**************************************
ディフューズ設定処理
***************************************/
void SetDiffuseNameEntryBG(float alpha)
{
	VERTEX_3D *p = NULL;

	vtxBuff->Lock(0, 0, (void**)&p, 0);

	for (int i = 0; i < NAMEENTRYBG_VTX_NUM*NAMEENTRYBG_VTX_NUM; i++)
	{
		p[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	}

	vtxBuff->Unlock();
}