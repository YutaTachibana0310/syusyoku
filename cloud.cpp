//==========================================
//
//雲処理[cloud.cpp]
//Author:GP11A341 21 立花雄太
//
//==========================================
#include "battleCamera.h"
#include "cloud.h"

/******************************************
マクロ定義
*******************************************/
#define CLOUD_TEXNAME		"data/TEXTURE/cloud.png"
#define CLOUD_SIZE			(512)
#define CLOUD_MAX			(1024)

/******************************************
構造体定義
*******************************************/

/******************************************
プロトタイプ宣言
*******************************************/
void MakeVertexCloud(void);

/******************************************
グローバル変数
*******************************************/
static const int widthTex = 512;
static const int heightTex = 512;

static LPDIRECT3DTEXTURE9 texture = NULL;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;
static D3DXMATRIX mtxWorld;
static D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, -200.0f, 0.0f);

static CLOUD* root = NULL;

#if 0
static LPDIRECT3DSURFACE9 surface = NULL;
static LPDIRECT3DSURFACE9 depthSurface = NULL;
static LPDIRECT3DSURFACE9 defaultSurface = NULL;
static LPDIRECT3DSURFACE9 defaultDepth = NULL;
#endif

/******************************************
初期化処理
*******************************************/
void InitCloud(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		texture = CreateTextureFromFile((LPSTR)CLOUD_TEXNAME, pDevice);
#if 0
		pDevice->GetRenderTarget(0, &defaultSurface);
		pDevice->GetDepthStencilSurface(&defaultDepth);

		//テクスチャ作成
		if (FAILED(D3DXCreateTexture(
			pDevice,
			widthTex,
			heightTex,
			0,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&texture)))
		{
			return;
		}

		//深度バッファ作成
		if (FAILED(pDevice->CreateDepthStencilSurface(
			widthTex,
			heightTex,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			true,
			&depthSurface,
			NULL
		)))
		{
			return;
		}
#endif
	}

	MakeVertexCloud();
}

/******************************************
終了処理
*******************************************/
void UninitCloud(int num)
{
	CLOUD *ptr = root, *tmp;
	while (ptr != NULL)
	{
		ptr->active = false;
		ptr = ptr->next;
	}

	if (num == 0)
	{
		SAFE_RELEASE(texture);
		ptr = root;
		while (ptr != NULL)
		{
			tmp = ptr->next;
			free(ptr);
			ptr = tmp;
		}
	}
}

/******************************************
更新処理
*******************************************/
void UpdateCloud(void)
{
	CLOUD* ptr = root;
	while (ptr != NULL)
	{
		ptr->pos.z -= 500.0f;

		if (ptr->pos.z <= -10000.0f)
		{
			ptr->active = false;
		}

		ptr = ptr->next;

	}

	for (int i = 0; i < 20; i++)
	{
		D3DXVECTOR3 offset = D3DXVECTOR3(RandomRange(-10000.0f, 10000.0f), RandomRange(-500.0f, -200.0f), 20000.0f);
		SetCloud(pos + offset);
	}
}

/******************************************
描画処理
*******************************************/
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, texture);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	for (CLOUD *ptr = root; ptr != NULL; ptr = ptr->next)
	{
		D3DXMatrixIdentity(&mtxWorld);

		GetInvRotBattleCamera(&mtxWorld);
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/******************************************
頂点情報作成
*******************************************/
void MakeVertexCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL)))
	{
		return;
	}

	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = pos + D3DXVECTOR3(-CLOUD_SIZE, CLOUD_SIZE, 0.0f);
	pVtx[1].vtx = pos + D3DXVECTOR3(CLOUD_SIZE, CLOUD_SIZE, 0.0f);
	pVtx[2].vtx = pos + D3DXVECTOR3(-CLOUD_SIZE, -CLOUD_SIZE, 0.0f);
	pVtx[3].vtx = pos + D3DXVECTOR3(CLOUD_SIZE, -CLOUD_SIZE, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 1.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxBuff->Unlock();

	return;
}

/**********************************************
関数	：void SetCloud(D3DXVRECTOR3 pos)
引数	：雲をセットしたい座標
戻り値	：void
説明	：雲をゲーム内にセットする関数
**********************************************/
void SetCloud(D3DXVECTOR3 pos)
{
	CLOUD *ptr = root, *prev = NULL;

	//ルートの雲生成
	if (root == NULL)
	{
		root = (CLOUD*)malloc(sizeof(CLOUD));
		root->pos = pos;
		root->prev = NULL;
		root->next = NULL;
		root->active = true;

		return;
	}

	//リスト内の使用可能な雲を探索
	while (ptr != NULL)
	{
		if (!ptr->active)
		{
			ptr->pos = pos;
			ptr->active = true;
			return;
		}

		if (ptr->next == NULL)
		{
			break;
		}

		ptr = ptr->next;
	}

	//リスト内に使用可能な雲がないので追加
	ptr->next = (CLOUD*)malloc(sizeof(CLOUD));
	ptr->next->pos = pos;
	ptr->next->active = true;
	ptr->next->next = NULL;
	ptr->next->prev = ptr;

	return;
}