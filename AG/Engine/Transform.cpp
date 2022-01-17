#include "DXUT.h"
#include "Transform.h"

USING(Engine)
Transform::Transform(void) :
	localPosition(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	worldPosition(0, 0, 0) // �̰͵��� ���ʱ�ȭ����
{
	D3DXMatrixIdentity(&worldMatrix);
	D3DXQuaternionIdentity(&quaternion);
	D3DXQuaternionIdentity(&curQuaternion);
	// ���� �� �ʱ�ȭ��
}

Transform::Transform(Transform* _parent) :
	localPosition(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	worldPosition(0, 0, 0)
{
	SetParent(_parent);
	D3DXMatrixIdentity(&worldMatrix);
	D3DXQuaternionIdentity(&quaternion);
	D3DXQuaternionIdentity(&curQuaternion);
	// �ʱ�ȭ�� �θ������� �ϴ»����ڳ�
}

Transform::~Transform(void)
{
}

void Transform::SetParent(Transform* _parent)
{
	ReleaseParent();
	parent = _parent;
	parent->AddReference();
}

void Transform::ReleaseParent(void)
{
	if (parent != nullptr)
	{
		parent->Release();
		parent = nullptr;
	}
}


void Transform::Rotate(RotType _rotType, float _angle)
{
	//reinterpret_cast�� ����
	D3DXVECTOR3 axis = *reinterpret_cast<D3DXVECTOR3*>(&worldMatrix.m[_rotType][0]); // ���� ��İ��� Vector3�� �ٲ۰ǰ�?

	D3DXQUATERNION rot;
	D3DXQuaternionRotationAxis(&rot, &axis, _angle); // ������ ������ ������
	quaternion *= rot; // ��� ȸ����Ű�°ǰ�? Unity Rotate�� ����Ѱǰ�
}

void Transform::Update(const FLOAT& dt)
{
	D3DXMATRIX S, R, T;
	D3DXMATRIX rx, ry, rz;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z); // ũ�ⰰ����

	D3DXQuaternionSlerp(&curQuaternion, &curQuaternion, &quaternion, dt * slerpSpeed); //Slerp�� �ֳ���?

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &curQuaternion); // ȸ��������

	D3DXMatrixTranslation(&T, localPosition.x, localPosition.y, localPosition.z); // ��ġ������

	worldMatrix = S * matRot * T;
	if (parent != nullptr)
	{
		worldMatrix *= parent->worldMatrix;
		worldPosition = *reinterpret_cast<D3DXVECTOR3*>(&worldMatrix._41);
	}
}

void Transform::Free(void)
{
	ReleaseParent();
}