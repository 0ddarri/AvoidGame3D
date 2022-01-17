#include "DXUT.h"
#include "Transform.h"

USING(Engine)
Transform::Transform(void) :
	localPosition(0, 0, 0),
	rotation(0, 0, 0),
	scale(1, 1, 1),
	worldPosition(0, 0, 0) // 이것들은 값초기화같고
{
	D3DXMatrixIdentity(&worldMatrix);
	D3DXQuaternionIdentity(&quaternion);
	D3DXQuaternionIdentity(&curQuaternion);
	// ㅇㅇ 다 초기화네
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
	// 초기화에 부모설정까지 하는생정자네
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
	//reinterpret_cast는 뭐야
	D3DXVECTOR3 axis = *reinterpret_cast<D3DXVECTOR3*>(&worldMatrix.m[_rotType][0]); // 대충 행렬값을 Vector3로 바꾼건가?

	D3DXQUATERNION rot;
	D3DXQuaternionRotationAxis(&rot, &axis, _angle); // 임의의 축으로 돌린다
	quaternion *= rot; // 계속 회전시키는건가? Unity Rotate랑 비슷한건가
}

void Transform::Update(const FLOAT& dt)
{
	D3DXMATRIX S, R, T;
	D3DXMATRIX rx, ry, rz;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z); // 크기같은데

	D3DXQuaternionSlerp(&curQuaternion, &curQuaternion, &quaternion, dt * slerpSpeed); //Slerp는 왜나와?

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &curQuaternion); // 회전같은데

	D3DXMatrixTranslation(&T, localPosition.x, localPosition.y, localPosition.z); // 위치같은데

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