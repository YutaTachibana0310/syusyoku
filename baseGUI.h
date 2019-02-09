//=====================================
//
//ベースGUIヘッダ[baseGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BASEGUI_H_
#define _BASEGUI_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
クラス宣言
***************************************/
class BaseGUI {
private:
	VERTEX_2D vtxWk[NUM_VERTEX];
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR2 size;
	float scale;

public:
	BaseGUI(LPSTR texName, float sizeX, float sizeY);
	void Load(LPSTR texName, float sizeX, float sizeY);
	void SetVertex(D3DXVECTOR3 pos, D3DXVECTOR2 scale = D3DXVECTOR2(1.0f, 1.0f));
	void SetTexture(int dixX, int divY, int pattern);
	void SetAlpha(float alpha);
	void SetLoop(float loopX, float loopY, float offsetX, float offsetY);
	void Draw();
	~BaseGUI();
};

#endif