#include "DXUT.h"
#include "GameObject.h"
#include "Layer.h"

USING(Engine)

void Layer::AddGameObject(const std::wstring& _objectKey, GameObject* _object) // ���� Ư�� ���̾ ���� �߰��ϴ°ǰ�
{
	if (_object == nullptr) // object null�̸� ��
		return;
	
	const auto& iter = gameobjectgroup.find(_objectKey); // const ���̸� �����ϰ� ����(�������) ���� ���縦 ���ؼ� �� ������
	if (iter == gameobjectgroup.end()) // ó������ ������ �����µ� ��ã����
		gameobjectgroup[_objectKey] = std::list<GameObject*>(); // ?? ����Ʈ�� �����ϱ� �ʱ�ȭ�� ���ش�

	gameobjectgroup[_objectKey].push_back(_object); // group�� _objectKey��°�� list�� ����
}

void Layer::Update(const FLOAT& dt)
{
	for (auto& objectlist : gameobjectgroup) // 
	{
		auto& iter = objectlist.second.begin(); // gameobjectlist ó��
		const auto& iter_end = objectlist.second.end(); // ��
		for (; iter != iter_end;) // while �����ΰ�? iter�� end�� �ٸ��� ��� ���´����ΰŰ�����
		{
			if ((*iter)->Update(dt) == OBJDEAD) // ������ ��������?
			{
				Safe_Release(*iter); // ����
				iter = objectlist.second.erase(iter); // ����Ʈ���� ����
			}
			else // �� ���������?
				++iter; // ��������?
		}
	}
}

void Layer::LateUpdate(const FLOAT& dt) // Update �Լ� ȣ��ǰ� �̰� ȣ��Ǵ°ǰ�
{
	for (auto& objectlist : gameobjectgroup) // gameobjectgroup ����
	{
		for (auto& object : objectlist.second) // �� �ȿ� list ����
		{
			if (object->GetActive() == true) // ���� ����������
				object->LateUpdate(dt); // LateUpdate ����
		}
	}
}

void Layer::ResetDevice(void) // ����̽� ���µǾ�����?
{
	for (auto& objectlist : gameobjectgroup)
	{
		for (auto& object : objectlist.second)
		{
			object->ResetDevice();
		}
	}
}

void Layer::LostDevice(void) // ����̽� �Ҿ��������?
{
	for (auto& objectlist : gameobjectgroup)
	{
		for (auto& object : objectlist.second)
		{
			object->LostDevice();
		}
	}
}

void Layer::Free(void)
{
	for (auto& objectlist : gameobjectgroup)
	{
		for (auto& object : objectlist.second)
		{
			Safe_Release(object);
		}
		objectlist.second.clear();
	}
	gameobjectgroup.clear();
}
