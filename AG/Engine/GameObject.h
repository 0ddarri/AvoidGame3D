#pragma once
#include "Base.h"

NAMESPACE(Engine)
class Layer;
class Component;

class GameObject : public Base
{
private:
protected:
	friend class Layer; // friend??????? ㅅㅂ 깐부?
	friend class CollisionManager;
	friend class ObjectManager;

	std::map<std::wstring, Component*> componentgroup; // 컴포넌트들 모아놓은거
	bool isActive = true; // 켜져있는지
	ColliderData colliderdata; // 충돌데이터인듯
public:

	explicit GameObject(void) {};
	virtual ~GameObject(void) {};
	
	void ResetDevice(void);
	void LostDevice(void);

	virtual void CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject) {}; // 충돌 이벤트?
	ColliderData* GetColliderData(void) { return &colliderdata; }; // 콜라이더 데이터 얻어오는건가
	bool GetActive(void) const; // 액티브 상태 받아오는건가

	Component* GetComponent(const std::wstring& componenttag) // 오 Unity에서 많이 봄
	{
		auto iter = componentgroup.find(componenttag); // componenygroup에서 찾는다
		if (iter != componentgroup.end()) // 찾으면
			return componentgroup[componenttag]; // 그거 리턴
		return nullptr; // 없으면 null 리턴
	};

	virtual INT Update(const FLOAT& dt); // 업데이트인듯
	virtual void LateUpdate(const FLOAT& dt) {}; // Late업데이트인듯
	virtual void Render(const FLOAT& dt); // 렌더링

	void SetActive(bool _active) { isActive = _active; }; // 액티브 상태 설정

	void Free(void) override; // 해제?
};
END
