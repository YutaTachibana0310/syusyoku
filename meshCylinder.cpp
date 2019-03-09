//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "meshCylinder.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHCYLINDER_TEXTURE        "data/TEXTURE/BG/circuit00.jpg"
#define MESHCYLINDER_CIRCUIT1		"data/TEXTURE/BG/circuit02.png"
#define MESHCYLINDER_CIRCUIT2		"data/TEXTURE/BG/circuit04.png"
#define MESHCYLINDER_BLOCKNUM		(50)
#define MESHCYLINDER_BLOCKSIZE		(800.0f)
#define MESHCYLINDER_RADIUS			(1000.0f)
#define MESHCYLINDER_TEXMAX			(3)
#define MESHCYLINDER_TEXSIZE		(0.1f)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9 texture[MESHCYLINDER_TEXMAX];
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static LPDIRECT3DINDEXBUFFER9 idxBuff;

static D3DXMATRIX mtxWorld;
static D3DXVECTOR3 pos;
static D3DXVECTOR3 rot;

static int numBlockX, numBlockZ;
static int numVertex;
static int numPolygon;
static int numIndex;
static float sizeBlockX, sizeBlockZ;
static float offset[MESHCYLINDER_TEXMAX];

static float offsetSpeed[MESHCYLINDER_TEXMAX] = { 0.0015f, 0.0017f, 0.002f};
static float speedMagni = 2.0f;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertexBufferCylinder(void);
void SetIndexBufferCylinder(void);
void SetTextureOffsetMeshCylinder(float offset);

//*****************************************************************************
// 初期化処理
//*****************************************************************************
void InitMeshCylinder(int num)
{
	static bool initialized = false;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pos = D3DXVECTOR3(0.0f, 0.0f, -2500.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	numBlockX = MESHCYLINDER_BLOCKNUM;
	numBlockZ = MESHCYLINDER_BLOCKNUM;

	numVertex = (numBlockX + 1) * (numBlockZ + 1);

	numIndex = (numBlockX + 1) * 2 * numBlockZ + (numBlockZ - 1) * 2;

	numPolygon = numBlockX * numBlockZ * 2 + (numBlockZ - 1) * 4;

	sizeBlockX = MESHCYLINDER_BLOCKSIZE;
	sizeBlockZ = MESHCYLINDER_BLOCKSIZE;

	if (!initialized)
	{
		texture[0] = CreateTextureFromFile((LPSTR)MESHCYLINDER_TEXTURE, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)MESHCYLINDER_CIRCUIT1, pDevice);
		texture[2] = CreateTextureFromFile((LPSTR)MESHCYLINDER_CIRCUIT2, pDevice);
		SetVertexBufferCylinder();
		SetIndexBufferCylinder();
		initialized = true;
	}
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitMeshCylinder(int num)
{

	if (num == 0)
	{
		for (int i = 0; i < MESHCYLINDER_TEXMAX; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
		SAFE_RELEASE(vtxBuff);
		SAFE_RELEASE(idxBuff);
	}
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateMeshCylinder(void)
{
	for (int i = 0; i < MESHCYLINDER_TEXMAX; i++)
	{
		offset[i] += offsetSpeed[i] * speedMagni;
		if (offset[i] > 1.0f)
		{
			offset[i] -= 1.0f;
		}
	}	
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	D3DXMatrixIdentity(&mtxWorld);

	//回転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//移動
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	//設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(idxBuff);
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, texture[0]);

	//描画
	SetTextureOffsetMeshCylinder(offset[0]);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVertex, 0, numPolygon);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetTexture(0, texture[1]);
	SetTextureOffsetMeshCylinder(offset[1]);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVertex, 0, numPolygon);

	pDevice->SetTexture(0, texture[2]);
	SetTextureOffsetMeshCylinder(offset[2]);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVertex, 0, numPolygon);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//*****************************************************************************
// 頂点作成処理
//*****************************************************************************
void SetVertexBufferCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL)))
	{
		return;
	}

	VERTEX_3D *pVtx;
	D3DXVECTOR3 basePos = D3DXVECTOR3(0.0f, -0.0f, 0.0f);
	D3DXVECTOR3 normal;
	float baseAngle = D3DXToRadian(360.0f / (numBlockX));

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXCOLOR diffuseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (int z = 0; z < (numBlockZ + 1); z++)
	{
		for (int x = 0; x < (numBlockX + 1); x++)
		{
			//座標設定
			pVtx[z * (numBlockX + 1) + x].vtx.x = cosf(baseAngle * x) * MESHCYLINDER_RADIUS;
			pVtx[z * (numBlockX + 1) + x].vtx.y = sinf(baseAngle * x) * -MESHCYLINDER_RADIUS;
			pVtx[z * (numBlockX + 1) + x].vtx.z = basePos.z + z * sizeBlockZ;

			//反射光設定
			pVtx[z * (numBlockZ * 1) + x].diffuse = diffuseColor;

			//UV座標設定
			pVtx[z * (numBlockX + 1) + x].tex.x = x * MESHCYLINDER_TEXSIZE;
			pVtx[z * (numBlockX + 1) + x].tex.y = z * MESHCYLINDER_TEXSIZE;
		}
	}

	D3DXVECTOR3 vec1, vec2;
	for (int z = 0; z < (numBlockZ + 1); z++)
	{
		for (int x = 0; x < (numBlockX + 1); x++)
		{
			//法線の設定
			if (z == numBlockZ)
			{
				pVtx[z * (numBlockX + 1) + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			else if (x == numBlockX)
			{
				vec1 = pVtx[z * (numBlockX + 1) + 1].vtx - pVtx[z * (numBlockX + 1) + x].vtx;
				vec2 = pVtx[(z + 1) * (numBlockX + 1)].vtx - pVtx[z * (numBlockX + 1) + x].vtx;
				D3DXVec3Cross(&normal, &vec2, &vec1);
				D3DXVec3Normalize(&pVtx[z * (numBlockX + 1) + x].nor, &normal);
			}
			else
			{
				vec1 = pVtx[z * (numBlockX + 1) + x + 1].vtx - pVtx[z * (numBlockX + 1) + x].vtx;
				vec2 = pVtx[(z + 1) * (numBlockX + 1) + x].vtx - pVtx[z * (numBlockX + 1) + x].vtx;
				D3DXVec3Cross(&normal, &vec2, &vec1);
				D3DXVec3Normalize(&pVtx[z * (numBlockX + 1) + x].nor, &normal);
			}

		}

	}

	vtxBuff->Unlock();
}

//*****************************************************************************
// インデックスバッファ設定処理
//*****************************************************************************
void SetIndexBufferCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateIndexBuffer(sizeof(DWORD) * numIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&idxBuff,
		NULL)))
	{
		return;
	}

	DWORD *pIdx;

	idxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int cnt = 0;
	for (int z = 0; z < numBlockZ; z++)
	{
		if (z > 0)
		{
			pIdx[cnt] = (z + 1) * (numBlockX + 1);
			cnt++;
		}

		for (int x = 0; x < (numBlockX + 1); x++)
		{
			pIdx[cnt] = (z + 1) * (numBlockX + 1) + x;
			cnt++;
			pIdx[cnt] = z * (numBlockX + 1) + x;
			cnt++;
		}

		if (z < (numBlockZ - 1))
		{
			pIdx[cnt] = z * (numBlockX + 1) + numBlockX;
			cnt++;
		}
	}

	idxBuff->Unlock();
}

//*****************************************************************************
// テクスチャオフセット処理
//*****************************************************************************
void SetTextureOffsetMeshCylinder(float offset)
{
	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int z = 0; z < (numBlockZ + 1); z++)
	{
		for (int x = 0; x < (numBlockX + 1); x++)
		{
			pVtx[z * (numBlockX + 1) + x].tex.x = MESHCYLINDER_TEXSIZE * x;
			pVtx[z * (numBlockX + 1) + x].tex.y = MESHCYLINDER_TEXSIZE * z + offset;
		}
	}

	vtxBuff->Unlock();
}