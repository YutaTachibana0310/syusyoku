//=============================================================================
//
// コライダー処理 [collider.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "collider.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPD3DXMESH meshSphere;		//球体メッシュ
static LPD3DXMESH meshCube;			//キューブメッシュ
static D3DMATERIAL9 material;		//当たり判定表示用マテリアル

/*****************************************************************************
初期化処理
*****************************************************************************/
void InitCollider(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//球体メッシュ作成
		D3DXCreateSphere(pDevice,
			1.0f,
			8,
			8,
			&meshSphere,
			NULL);

		//キューブメッシュ作成
		D3DXCreateBox(pDevice,
			1.0f,
			1.0f,
			1.0f,
			&meshCube,
			NULL);

		//マテリアル作成
		ZeroMemory(&material, sizeof(material));
		material.Diffuse.r = 1.0f;
		material.Diffuse.g = 0.0f;
		material.Diffuse.b = 0.0f;
		material.Diffuse.a = 1.0f;

		material.Ambient.r = 1.0f;
		material.Ambient.g = 0.0f;
		material.Ambient.b = 0.0f;
		material.Ambient.a = 1.0f;

		material.Specular.r = 1.0f;
		material.Specular.g = 0.0f;
		material.Specular.b = 0.0f;
		material.Specular.a = 1.0f;

		material.Emissive.r = 1.0f;
		material.Emissive.g = 0.0f;
		material.Emissive.b = 0.0f;
		material.Emissive.a = 1.0f;

		initialized = true;

	}
}

/*****************************************************************************
終了処理
*****************************************************************************/
void UninitCollider(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(meshSphere);
		SAFE_RELEASE(meshCube);
	}
}


/*****************************************************************************
バウンディングスフィア描画処理
*****************************************************************************/
void DrawBoundingSphere(const COLLIDER_SPHERE *s)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxScale, mtxWorld;
	D3DMATERIAL9 matDef;

	//get defaultMaterial
	pDevice->GetMaterial(&matDef);

	//set renderstate & material
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetMaterial(&material);
	pDevice->SetTexture(0, NULL);

	//identify
	D3DXMatrixIdentity(&mtxWorld);

	//scaling
	D3DXMatrixScaling(&mtxScale, s->radius, s->radius, s->radius);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	//translate
	D3DXMatrixTranslation(&mtxTranslate, s->pos->x + s->offset.x, s->pos->y + s->offset.y, s->pos->z + s->offset.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
	
	//set world
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//draw
	meshSphere->DrawSubset(0);

	//reset renderstate & material0
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetMaterial(&matDef);
}

/*****************************************************************************
バウンディングキューブ描画処理
*****************************************************************************/
void DrawBoundingCube(const COLLIDER_CUBE *cube)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxScale, mtxWorld;
	D3DMATERIAL9 matDef;

	//get defaultMaterial
	pDevice->GetMaterial(&matDef);

	//set renderstate & material
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pDevice->SetMaterial(&material);
	pDevice->SetTexture(0, NULL);

	//identify
	D3DXMatrixIdentity(&mtxWorld);

	//scaling
	D3DXMatrixScaling(&mtxScale, cube->length.x * 2, cube->length.y * 2, cube->length.z * 2);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	//translate
	D3DXMatrixTranslation(&mtxTranslate, cube->pos->x + cube->offset.x, cube->pos->y + cube->offset.y, cube->pos->z + cube->offset.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
	
	//set world
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//draw
	meshCube->DrawSubset(0);

	//reset renderstate & material0
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetMaterial(&matDef);
}

/*****************************************************************************
関数名	：bool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, TRIANGLE tri, D3DXVECTOR3 *out)
引数1	：D3DXVECTOR3 start	…　線分の始点
引数2	：D3DXVECTOR3 end	…　線分の終点
引数3	：TRIANGLE tri		…　三角形ポリゴン
引数4	：D3DXVECTOR3 *out	…　交点を格納するポインタ
説明	：線分と三角形ポリゴンの当たり判定
*****************************************************************************/
bool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, TRIANGLE tri, D3DXVECTOR3 *out)
{
	//線分の始点、終点とフィールドの法線ベクトルとの内積を計算
	D3DXVECTOR3 v1 = start - tri.a;
	D3DXVECTOR3 v2 = end - tri.a;

	float dotV1 = D3DXVec3Dot(&tri.nor, &v1);
	float dotV2 = D3DXVec3Dot(&tri.nor, &v2);

	//平面と線分が交差していない(内積同士の積の符号が正である)場合、falseをリターン
	if (dotV1 * dotV2 > 0)
	{
		return false;
	}

	//線分と平面の交点を計算
	D3DXVECTOR3 line = end - start;	//線分を表すベクトル

	float ratio = fabsf(dotV1) / (fabsf(dotV1) + fabsf(dotV2));

	float x = start.x + (line.x * ratio);	//x座標
	float y = start.y + (line.y * ratio);	//y座標
	float z = start.z + (line.z * ratio);	//z座標
	D3DXVECTOR3 p = D3DXVECTOR3(x, y, z);	//交点

	//交点がポリゴン領域内かチェック
	D3DXVECTOR3 n1, n2, n3;

	D3DXVec3Cross(&n1, &(tri.b - tri.a), &(p - tri.a));	//フィールドの辺と、頂点→交点ベクトルの法線ベクトルを計算
	D3DXVec3Cross(&n2, &(tri.c - tri.b), &(p - tri.b));
	D3DXVec3Cross(&n3, &(tri.a - tri.c), &(p - tri.c));

	float dot_n12 = D3DXVec3Dot(&n1, &n2);
	float dot_n23 = D3DXVec3Dot(&n2, &n3);

	//全ての法線ベクトルが同じ向き（内積が負にならない）であれば交点はフィールド内にある
	if (dot_n12 >= 0 && dot_n23 >= 0)
	{
		*out = p;
		return true;
	}

	return false;
}

/*****************************************************************************
関数名	：bool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, PLANE plane, D3DXVECTOR3 *out)
引数1	：D3DXVECTOR3 start	…　線分の始点
引数2	：D3DXVECTOR3 end	…　線分の終点
引数3	：TRIANGLE tri		…　四角形ポリゴン半径
引数4	：D3DXVECTOR3 *out	…　交点を格納するポインタ
説明	：三角形ポリゴンと線分の当たり判定
*****************************************************************************/
bool CheckHitPlaneAndLine(D3DXVECTOR3 start, D3DXVECTOR3 goal, PLANE plane, D3DXVECTOR3 *out)
{
	//線分の始点、終点とフィールドの法線ベクトルとの内積を計算
	D3DXVECTOR3 v1 = start - plane.topL;
	D3DXVECTOR3 v2 = goal - plane.topL;

	float dotV1 = D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &v1);
	float dotV2 = D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &v2);

	//平面と線分が交差していない(内積同士の積の符号が正である)場合、falseをリターン
	if (dotV1 * dotV2 > 0)
	{
		return false;
	}

	//線分と平面の交点を計算
	D3DXVECTOR3 line = goal - start;	//線分を表すベクトル

	float ratio = fabsf(dotV1) / (fabsf(dotV1) + fabsf(dotV2));

	float x = start.x + (line.x * ratio);	//x座標
	float y = start.y + (line.y * ratio);	//y座標
	float z = start.z + (line.z * ratio);	//z座標
	D3DXVECTOR3 p = D3DXVECTOR3(x, y, z);			//交点

											//交点がポリゴン領域内かチェック
	D3DXVECTOR3 n1, n2, n3, n4;

	D3DXVec3Cross(&n1, &(plane.bottomL - plane.topL), &(p - plane.bottomL));	//フィールドの辺と、頂点→交点ベクトルの法線ベクトルを計算
	D3DXVec3Cross(&n2, &(plane.bottomR - plane.bottomL), &(p - plane.bottomR));
	D3DXVec3Cross(&n3, &(plane.topR - plane.bottomR), &(p - plane.topR));
	D3DXVec3Cross(&n4, &(plane.topL- plane.topR), &(p - plane.topL));

	float dot_n12 = D3DXVec3Dot(&n1, &n2);
	float dot_n23 = D3DXVec3Dot(&n2, &n3);
	float dot_n34 = D3DXVec3Dot(&n3, &n4);

	//全ての法線ベクトルが同じ向き（内積が負にならない）であれば交点はフィールド内にある
	if (dot_n12 >= 0 && dot_n23 >= 0 && dot_n34 >= 0)
	{
		*out = p;
		return true;
	}

	return false;
}

/*****************************************************************************
関数名	：bool ChechHitBoundingSphere(SPHERE s1, SPHERE s2)
引数1	：SPHERE s1　…　バウンディングスフィア1
引数2	：SPHERE s2　…　バウンディングスフィア2
説明	：バウンディングスフィアの当たり判定
*****************************************************************************/
bool CheckHitBoundingSphere(const COLLIDER_SPHERE *s1, const COLLIDER_SPHERE *s2)
{
	if (!s1->active || !s2->active)
	{
		//return false;
	}

	D3DXVECTOR3 d = (*s2->pos + s2->offset) -(*s1->pos + s1->offset);
	float lenghtSq = D3DXVec3LengthSq(&d);

	return (lenghtSq > (s1->radius + s2->radius) * (s1->radius + s2->radius)) ? false : true;
}

/*****************************************************************************
関数名	：bool ChechHitBoundingCube(SPHERE s1, SPHERE s2)
引数1	：SPHERE s1　…　バウンディングスフィア1
引数2	：SPHERE s2　…　バウンディングスフィア2
説明	：バウンディングスフィアの当たり判定
*****************************************************************************/
bool ChechHitBoundingCube(const COLLIDER_CUBE *c1, const COLLIDER_CUBE *c2)
{
	
	D3DXVECTOR3 pos1 = *(c1->pos) + c1->offset;
	D3DXVECTOR3 pos2 = *(c2->pos) + c2->offset;

	if (pos1.x + c1->length.x < pos2.x - c2->length.x || pos1.x - c1->length.x > pos2.x + c2->length.x)
		return false;

	if (pos1.y + c1->length.y < pos2.y - c2->length.y || pos1.y - c1->length.y > pos2.y + c2->length.y)
		return false;

	if (pos1.z + c1->length.z < pos2.z - c2->length.z || pos1.z - c1->length.z > pos2.z + c2->length.z)
		return false;

	return true;
}