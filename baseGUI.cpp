//=====================================
//
//ベースGUI処理[baseGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "baseGUI.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
コンストラクタ
***************************************/
BaseGUI::BaseGUI(LPSTR texName, float sizeX, float sizeY)
{
	if (texture != NULL)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	texture = CreateTextureFromFile(texName, pDevice);
	size = D3DXVECTOR2(sizeX, sizeY);

	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;
	
	this->SetTexture(1, 1, 0);

	this->SetAlpha(1.0f);

	this->SetVertex(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f));

	scale = 1.0f;
}

/**************************************
デストラクタ
***************************************/
BaseGUI::~BaseGUI()
{
	SAFE_RELEASE(texture);
}

/**************************************
読み込み処理
***************************************/
void BaseGUI::Load(LPSTR texName, float sizeX, float sizeY)
{
	if (texture != NULL)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	texture = CreateTextureFromFile(texName, pDevice);
	size = D3DXVECTOR2(sizeX, sizeY);

	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	this->SetTexture(1, 1, 0);

	this->SetAlpha(1.0f);

	this->SetVertex(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f));

	scale = 1.0f;
}

/**************************************
頂点設定処理
**************************************/
void BaseGUI::SetVertex(D3DXVECTOR3 pos, D3DXVECTOR2 scale)
{
	vtxWk[0].vtx = pos + D3DXVECTOR3(-size.x * scale.x, -size.y * scale.y, 0.0f);
	vtxWk[1].vtx = pos + D3DXVECTOR3(size.x * scale.x, -size.y * scale.y, 0.0f);
	vtxWk[2].vtx = pos + D3DXVECTOR3(-size.x * scale.x, size.y * scale.y, 0.0f);
	vtxWk[3].vtx = pos + D3DXVECTOR3(size.x * scale.x, size.y * scale.y, 0.0f);
}

/**************************************
テクスチャ座標設定処理
**************************************/
void BaseGUI::SetTexture(int divX, int divY, int pattern)
{
	float sizeX = 1.0f / divX;
	float sizeY = 1.0f / divY;
	int x = pattern % divX;
	int y = pattern / divX;

	vtxWk[0].tex = D3DXVECTOR2(x * sizeX, y * sizeY);
	vtxWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, y * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(x * sizeX, (y + 1)* sizeY);
	vtxWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, (y + 1) * sizeY);
}

/**************************************
アルファ設定処理
**************************************/
void BaseGUI::SetAlpha(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}

/**************************************
描画処理
**************************************/
void BaseGUI::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, texture);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}
