//=============================================================================
//
// �R���C�_�[���� [collider.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "collider.h"
/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾��`
*****************************************************************************/
void UpdateCollision(void)
{


}

/*****************************************************************************
�֐���	�Fbool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, TRIANGLE tri, D3DXVECTOR3 *out)
����1	�FD3DXVECTOR3 start	�c�@�����̎n�_
����2	�FD3DXVECTOR3 end	�c�@�����̏I�_
����3	�FTRIANGLE tri		�c�@�O�p�`�|���S��
����4	�FD3DXVECTOR3 *out	�c�@��_���i�[����|�C���^
����	�F�����ƎO�p�`�|���S���̓����蔻��
*****************************************************************************/
bool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, TRIANGLE tri, D3DXVECTOR3 *out)
{
	//�����̎n�_�A�I�_�ƃt�B�[���h�̖@���x�N�g���Ƃ̓��ς��v�Z
	D3DXVECTOR3 v1 = start - tri.a;
	D3DXVECTOR3 v2 = end - tri.a;

	float dotV1 = D3DXVec3Dot(&tri.nor, &v1);
	float dotV2 = D3DXVec3Dot(&tri.nor, &v2);

	//���ʂƐ������������Ă��Ȃ�(���ϓ��m�̐ς̕��������ł���)�ꍇ�Afalse�����^�[��
	if (dotV1 * dotV2 > 0)
	{
		return false;
	}

	//�����ƕ��ʂ̌�_���v�Z
	D3DXVECTOR3 line = end - start;	//������\���x�N�g��

	float ratio = fabsf(dotV1) / (fabsf(dotV1) + fabsf(dotV2));

	float x = start.x + (line.x * ratio);	//x���W
	float y = start.y + (line.y * ratio);	//y���W
	float z = start.z + (line.z * ratio);	//z���W
	D3DXVECTOR3 p = D3DXVECTOR3(x, y, z);	//��_

	//��_���|���S���̈�����`�F�b�N
	D3DXVECTOR3 n1, n2, n3;

	D3DXVec3Cross(&n1, &(tri.b - tri.a), &(p - tri.a));	//�t�B�[���h�̕ӂƁA���_����_�x�N�g���̖@���x�N�g�����v�Z
	D3DXVec3Cross(&n2, &(tri.c - tri.b), &(p - tri.b));
	D3DXVec3Cross(&n3, &(tri.a - tri.c), &(p - tri.c));

	float dot_n12 = D3DXVec3Dot(&n1, &n2);
	float dot_n23 = D3DXVec3Dot(&n2, &n3);

	//�S�Ă̖@���x�N�g�������������i���ς����ɂȂ�Ȃ��j�ł���Ό�_�̓t�B�[���h���ɂ���
	if (dot_n12 >= 0 && dot_n23 >= 0)
	{
		*out = p;
		return true;
	}

	return false;
}

/*****************************************************************************
�֐���	�Fbool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, PLANE plane, D3DXVECTOR3 *out)
����1	�FD3DXVECTOR3 start	�c�@�����̎n�_
����2	�FD3DXVECTOR3 end	�c�@�����̏I�_
����3	�FTRIANGLE tri		�c�@�l�p�`�|���S�����a
����4	�FD3DXVECTOR3 *out	�c�@��_���i�[����|�C���^
����	�F�l�p�`�|���S���Ɛ����̓����蔻��
*****************************************************************************/
bool CheckHitPlaneAndLine(D3DXVECTOR3 start, D3DXVECTOR3 goal, PLANE plane, D3DXVECTOR3 *out)
{
	//�����̎n�_�A�I�_�ƃt�B�[���h�̖@���x�N�g���Ƃ̓��ς��v�Z
	D3DXVECTOR3 v1 = start - plane.topL;
	D3DXVECTOR3 v2 = goal - plane.topL;

	float dotV1 = D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &v1);
	float dotV2 = D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &v2);

	//���ʂƐ������������Ă��Ȃ�(���ϓ��m�̐ς̕��������ł���)�ꍇ�Afalse�����^�[��
	if (dotV1 * dotV2 > 0)
	{
		return false;
	}

	//�����ƕ��ʂ̌�_���v�Z
	D3DXVECTOR3 line = goal - start;	//������\���x�N�g��

	float ratio = fabsf(dotV1) / (fabsf(dotV1) + fabsf(dotV2));

	float x = start.x + (line.x * ratio);	//x���W
	float y = start.y + (line.y * ratio);	//y���W
	float z = start.z + (line.z * ratio);	//z���W
	D3DXVECTOR3 p = D3DXVECTOR3(x, y, z);			//��_

											//��_���|���S���̈�����`�F�b�N
	D3DXVECTOR3 n1, n2, n3, n4;

	D3DXVec3Cross(&n1, &(plane.bottomL - plane.topL), &(p - plane.bottomL));	//�t�B�[���h�̕ӂƁA���_����_�x�N�g���̖@���x�N�g�����v�Z
	D3DXVec3Cross(&n2, &(plane.bottomR - plane.bottomL), &(p - plane.bottomR));
	D3DXVec3Cross(&n3, &(plane.topR - plane.bottomR), &(p - plane.topR));
	D3DXVec3Cross(&n4, &(plane.topL- plane.topR), &(p - plane.topL));

	float dot_n12 = D3DXVec3Dot(&n1, &n2);
	float dot_n23 = D3DXVec3Dot(&n2, &n3);
	float dot_n34 = D3DXVec3Dot(&n3, &n4);

	//�S�Ă̖@���x�N�g�������������i���ς����ɂȂ�Ȃ��j�ł���Ό�_�̓t�B�[���h���ɂ���
	if (dot_n12 >= 0 && dot_n23 >= 0 && dot_n34 >= 0)
	{
		*out = p;
		return true;
	}

	return false;
}

/*****************************************************************************
�֐���	�Fbool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, PLANE plane, D3DXVECTOR3 *out)
����1	�FD3DXVECTOR3 start	�c�@�����̎n�_
����2	�FD3DXVECTOR3 end	�c�@�����̏I�_
����3	�FTRIANGLE tri		�c�@�l�p�`�|���S�����a
����4	�FD3DXVECTOR3 *out	�c�@��_���i�[����|�C���^
����	�F�l�p�`�|���S���Ɛ����̓����蔻��
*****************************************************************************/
bool ChechHitBoundingSphere(SPHERE s1, SPHERE s2)
{
	if (!s1.active || !s2.active)
	{
		return false;
	}

	D3DXVECTOR3 d = (s2.pos - s1.pos);
	float lenghtSq = D3DXVec3LengthSq(&d);

	return (lenghtSq > (s1.radius + s2.radius) * (s1.radius + s2.radius)) ? false : true;
}