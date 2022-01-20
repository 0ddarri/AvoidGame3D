#pragma once
#include "Component.h"

NAMESPACE(Engine)
class Transform final : public Component // Transform 컴포넌트
{
private:
protected:
public:
	Transform* parent = nullptr; // 부모?
	explicit Transform(void);
	explicit Transform(Transform* _parent); 
	virtual ~Transform(void);

public:
	enum RotType : unsigned short { RIGHT, UP, LOOK }; // ??

	D3DXVECTOR3 localPosition; // 로컬위치
	D3DXVECTOR3 rotation; // 회전

	D3DXQUATERNION quaternion; // 사원수네
	D3DXQUATERNION curQuaternion; // 이건 뭔사원수야

	D3DXVECTOR3 scale; // 스케일

	D3DXMATRIX worldMatrix; // 
	D3DXVECTOR3 worldPosition; // 월드포지션

	FLOAT slerpSpeed = 8.f; // slerp 속도래
	bool quaternionMode = true; // 쿼터니언모드래 slerp 쿼터니언모드설정인가

	void Update(const FLOAT& dt) override;

	void SetParent(Transform* _parent); // 부모 설정
	void ReleaseParent(void); // 부모 해제

	void Rotate(RotType _rotType, float _angle); // 회전 
	void RotateAxis(D3DXVECTOR3 _axis, float _angle); // 회전

	bool Lerp(D3DXVECTOR3& _position, const D3DXVECTOR3& _destination, const FLOAT& _dt, const FLOAT& _distance = 0.5f) // Lerp
	{
		D3DXVec3Lerp(&_position, &_position, &_destination, _dt); //lerp한다
		if (D3DXVec3Length(&D3DXVECTOR3(_position - _destination)) < _distance) // position과 destination 사이 거리가 distance보다 작아지면
		{
			return true; // true 리턴
		}
		return false;
	}

	// 회전행렬을 오일러각으로 변환하는앤가
	static D3DXVECTOR3 rotationMatrixToEulerAngles(D3DXMATRIX& R) // ??
	{
		float sy = sqrt(R._11 * R._11 + R._21 * R._21);

		bool singular = sy < 1e-6; // If

		float x, y, z;
		if (!singular)
		{
			x = atan2(R._32, R._33);
			y = atan2(-R._31, sy);
			z = atan2(R._21, R._11);
		}
		else
		{
			x = atan2(-R._23, R._22);
			y = atan2(-R._31, sy);
			z = 0;
		}

		D3DXVECTOR3 result{ x,y,z };
		return result;
	}

	void Free(void) override;
};
END