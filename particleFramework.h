//=====================================
//
//パーティクルフレームワークヘッダ[particleFramework.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PARTICLEFRAMEWORK_H_
#define _PARTICLEFRAMEWORK_H_

#include "main.h"
#include "particleManager.h"

/**************************************
プロトタイプ宣言
***************************************/
//単位頂点バッファ作成処理
void MakeParticleVertexBuffer(VERTEX_PARTICLE *vtx, float particleSize, LPDIRECT3DVERTEXBUFFER9 *buff);

//UV頂点バッファ作成処理
void MakeParticleUVBuffer(int size, VERTEX_UV *uv, LPDIRECT3DVERTEXBUFFER9 *buff);

//ワールド変換バッファ作成処理
void MakeParticleWorldBuffer(int size, D3DXMATRIX *world, LPDIRECT3DVERTEXBUFFER9 *buff);

//カラーバッファ作成処理
void MakeParticleColorBuffer(int size, VERTEX_COLOR *color, LPDIRECT3DVERTEXBUFFER9 *buff);

#endif