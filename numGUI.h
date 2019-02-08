//=====================================
//
//ナンバーGUIヘッダ[numGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _NUMGUI_H_
#define _NUMGUI_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define NUMTEX_DIV_X			(5)
#define NUMTEX_DIV_Y			(2)
#define NUMTEX_SCORE_NAME		"data/TEXTURE/UI/scoreNum.png"
#define NUMTEX_LOCKON_NAME		"data/TEXTURE/UI/lockonNum.png"
#define NUMTEX_TIME_NAME		"data/TEXTURE/UI/timeNum.png"
#define NUMTEX_RANKING_NAME		"data/TEXTURE/UI/rankingNum.png"

enum DefineNumTexture
{
	NumScore,
	NumLockon,
	NumBonusTime,
	NumRanking,
	NumDefineMax
};

/**************************************
構造体定義
***************************************/
class NumGUI
{
private:
	static NumGUI *instance;					//シングルトンインスタンス
	LPDIRECT3DTEXTURE9 texture[NumDefineMax];	//テクスチャ
	VERTEX_2D vtxWk[NUM_VERTEX];				//頂点情報
	float scale;

	NumGUI() = default;							
	~NumGUI() = default;

public:
	/*インスタンス作成処理*/
	static void Create(void)
	{
		if (instance == nullptr)
		{
			instance = new NumGUI;

			//テクスチャ読み込み
			LPDIRECT3DDEVICE9 pDevice = GetDevice();
			instance->texture[0] = CreateTextureFromFile((LPSTR)NUMTEX_SCORE_NAME, pDevice);
			instance->texture[1] = CreateTextureFromFile((LPSTR)NUMTEX_LOCKON_NAME, pDevice);
			instance->texture[2] = CreateTextureFromFile((LPSTR)NUMTEX_TIME_NAME, pDevice);
			instance->texture[3] = CreateTextureFromFile((LPSTR)NUMTEX_RANKING_NAME, pDevice);

			//描画優先順位設定
			instance->vtxWk[0].rhw =
				instance->vtxWk[1].rhw =
				instance->vtxWk[2].rhw =
				instance->vtxWk[3].rhw = 1.0f;

			//ディフューズ初期化
			instance->vtxWk[0].diffuse =
				instance->vtxWk[1].diffuse =
				instance->vtxWk[2].diffuse =
				instance->vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	/*インスタンス取得処理*/
	static NumGUI* GetInstance(void)
	{
		return instance;
	}

	/*インスタンス解放処理*/
	static void Release(void)
	{
		for (int i = 0; i < NumDefineMax; i++)
		{
			SAFE_RELEASE(instance->texture[i]);
		}

		delete instance;
		instance = nullptr;
	}

	/*頂点座標設定処理*/
	void SetVertex(D3DXVECTOR3 pos, float sizeX, float sizeY)
	{
		instance->vtxWk[0].vtx = D3DXVECTOR3(-sizeX, -sizeY, 0.0f) + pos;
		instance->vtxWk[1].vtx = D3DXVECTOR3(sizeX, -sizeY, 0.0f) + pos;
		instance->vtxWk[2].vtx = D3DXVECTOR3(-sizeX, sizeY, 0.0f) + pos;
		instance->vtxWk[3].vtx = D3DXVECTOR3(sizeX, sizeY, 0.0f) + pos;
	}

	/*UV座標設定処理*/
	void SetTexture(int num)
	{
		float sizeX = 1.0f / NUMTEX_DIV_X;
		float sizeY = 1.0f / NUMTEX_DIV_Y;
		int x = num % NUMTEX_DIV_X;
		int y = num / NUMTEX_DIV_X;

		instance->vtxWk[0].tex = D3DXVECTOR2(x * sizeX, y * sizeY);
		instance->vtxWk[1].tex = D3DXVECTOR2((x + 1) * sizeX, y * sizeY);
		instance->vtxWk[2].tex = D3DXVECTOR2(x * sizeX, (y + 1) * sizeY);
		instance->vtxWk[3].tex = D3DXVECTOR2((x + 1) * sizeX, (y + 1) * sizeY);
	}

	/*アルファ設定処理*/
	void SetAlpha(float alpha)
	{
		instance->vtxWk[0].diffuse =
			instance->vtxWk[1].diffuse =
			instance->vtxWk[2].diffuse =
			instance->vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	}

	/*描画処理*/
	void Draw(DefineNumTexture def)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0, instance->texture[def]);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
	}
};

#endif