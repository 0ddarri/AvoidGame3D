#pragma once
#include "Base.h"

NAMESPACE(Engine)
class Layer;
class Component;

class GameObject : public Base
{
private:
protected:
	friend class Layer; // friend??????? ���� ���?
	friend class CollisionManager;
	friend class ObjectManager;

	std::map<std::wstring, Component*> componentgroup; // ������Ʈ�� ��Ƴ�����
	bool isActive = true; // �����ִ���
	ColliderData colliderdata; // �浹�������ε�
public:

	explicit GameObject(void) {};
	virtual ~GameObject(void) {};
	
	void ResetDevice(void);
	void LostDevice(void);

	virtual void CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject) {}; // �浹 �̺�Ʈ?
	ColliderData* GetColliderData(void) { return &colliderdata; }; // �ݶ��̴� ������ �����°ǰ�
	bool GetActive(void) const; // ��Ƽ�� ���� �޾ƿ��°ǰ�

	Component* GetComponent(const std::wstring& componenttag) // �� Unity���� ���� ��
	{
		auto iter = componentgroup.find(componenttag); // componenygroup���� ã�´�
		if (iter != componentgroup.end()) // ã����
			return componentgroup[componenttag]; // �װ� ����
		return nullptr; // ������ null ����
	};

	virtual INT Update(const FLOAT& dt); // ������Ʈ�ε�
	virtual void LateUpdate(const FLOAT& dt) {}; // Late������Ʈ�ε�
	virtual void Render(const FLOAT& dt); // ������

	void SetActive(bool _active) { isActive = _active; }; // ��Ƽ�� ���� ����

	void Free(void) override; // ����?
};
END
