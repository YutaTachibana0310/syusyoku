//=====================================
//
//�X���[���G�l�~�[���f������[smallEnemyModel.cpp]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#include "smallEnemyModel.h"
#include "playerModel.h"
#include "targetSite.h"

/**************************************
�}�N����`
***************************************/
#define SMALLENEMY_MODEL_NAME		"data/MODEL/smallEnemy.x"

/**************************************
�\���̒�`
***************************************/
typedef void(*funcSmallEnemy)(SMALLENEMYMODEL *enemy);

/**************************************
�O���[�o���ϐ�
***************************************/
static LPD3DXMESH mesh;
static LPD3DXBUFFER materials;
static DWORD numMaterial;
static LPDIRECT3DTEXTURE9 texture;

static SMALLENEMYMODEL smallEnemy[SMALLENEMY_MAX];

//�X�V�����e�[�u��
static funcSmallEnemy Update[SmallEnemyStateMax] = {
	UpdateSmallEnemyMove,
	UpdateSmallEnemyAttack,
	UpdateSmallEnemyEscape
};

//���ꏈ���e�[�u��
static funcSmallEnemy Enter[SmallEnemyStateMax] = {
	EnterSmallEnemyMove,
	EnterSmallEnemyAttack,
	EnterSmallEnemyEscape
};

//�ޏꏈ���e�[�u��
static funcSmallEnemy Exit[SmallEnemyStateMax] = {
	ExitSmallEnemyMove,
	ExitSmallEnemyAttack,
	ExitSmallEnemyEscape
};

/**************************************
�v���g�^�C�v�錾
***************************************/
void CreateSmallEnemyVertexBuffer(void);

/**************************************
����������
***************************************/
void InitSmallEnemyModel(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		if (FAILED(D3DXLoadMeshFromX(SMALLENEMY_MODEL_NAME,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&materials,
			NULL,
			&numMaterial,
			&mesh)))
		{
			return;
		}



	}

	SMALLENEMYMODEL *ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
		ptr->hp = 0.5f;
	}


}

/**************************************
�I������
***************************************/
void UninitSmallEnemyModel(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(mesh);
		SAFE_RELEASE(materials);

		
	}

	SMALLENEMYMODEL *ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
	}
}

/**************************************
�X�V����
***************************************/
void UpdateSmallEnemyModel(void)
{
	
}

/**************************************
�`�揈��
***************************************/
void DrawSmallEnemyModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);

	SMALLENEMYMODEL *ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		//rotate
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->rot.y, ptr->rot.x, ptr->rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//translate
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//set world
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//draw
		pMaterial = (D3DXMATERIAL*)materials->GetBufferPointer();
		for (int j = 0; j < (int)numMaterial; j++)
		{
			pDevice->SetMaterial(&pMaterial[j].MatD3D);
			
			mesh->DrawSubset(j);
		}


		pDevice->SetMaterial(&matDef);
	}
}

/**************************************
��ԑJ�ڏ���
***************************************/
void ChangeStateSmallEnemy(SMALLENEMYMODEL *ptr, int next)
{
	Exit[ptr->state](ptr);
	ptr->state = next;
	Enter[ptr->state](ptr);
}

/**************************************
�A�h���X�擾����
***************************************/
SMALLENEMYMODEL *GetSmallEnemyAdr(int num)
{
	return &smallEnemy[num];
}

/**************************************
�X���[���G�l�~�[�Z�b�g����
***************************************/

/**************************************
�X���[���G�l�~�[���b�N�I������
***************************************/
void LockonSmallEnemy(void)
{
	SMALLENEMYMODEL *ptr = &smallEnemy[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
		{
			continue;
		}

		ptr = &smallEnemy[0];
		for (int j = 0; j < SMALLENEMY_MAX; j++, ptr++)
		{
			if (!ptr->active)
			{
				continue;
			}

			if (CollisionTargetSite(i, &ptr->pos))
			{
				AddRockonTarget(i, &ptr->pos, &ptr->active, &ptr->hp);
			}
		}
	}
}