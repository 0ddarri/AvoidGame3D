#pragma once
#include "SceneManager.h"
#include "Layer.h"
#include "Singleton.h"
NAMESPACE(Engine)

class Layer;
class SceneManager;
class ObjectManager final : public Singleton<ObjectManager>
{
public:
    template<class T>
    T* GetActiveFalsedObject(LAYERKEY _layerKey, const std::wstring& _objecttag)
    {
        Layer* l = SceneManager::GetInstance()->GetCurrentSceneLayer(_layerKey); // ������� ���̾ �޾ƿ�
        for (auto& iter : l->gameobjectgroup[_objecttag]) // ���ӿ�����Ʈ�� for ������
        {
            if (iter->isActive == false) // ��Ȱ��ȭ�Ǿ��ִ¾� ã��
                return dynamic_cast<T*>(iter); // �� ��ȯ
        }
        return nullptr;
    }

    template<class T>
    T* AddObjectAtLayer(LAYERKEY _layerKey, const std::wstring& _objecttag) // ������Ʈ�� �߰��ϴ� ���ΰ���
    {
        T* object = new T(); // ������Ʈ ����
        Layer* l = SceneManager::GetInstance()->GetCurrentSceneLayer(_layerKey);// ���̾� ������
        l->AddGameObject(_objecttag, object); // �� ���̾ �߰���
        return dynamic_cast<T*>(object);
    }

    // ��Ȱ��ȭ�� ������Ʈ�� �������� �¸� ��ȯ�Ѵ� 
    template<class T>
    T* CheckActiveFalsedObjectAndSpawn(LAYERKEY _layerKey, const std::wstring& _objecttag)
    {
        T* object = GetActiveFalsedObject<T>(_layerKey, _objecttag); // ���� ������
        if (object == nullptr) // null�̸�
        {
            return AddObjectAtLayer<T>(_layerKey, _objecttag); // �����
        }
        return object; // �ƴϸ� �� ����?
    }
};
END