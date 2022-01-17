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
        Layer* l = SceneManager::GetInstance()->GetCurrentSceneLayer(_layerKey); // 현재씬의 레이어를 받아와
        for (auto& iter : l->gameobjectgroup[_objecttag]) // 게임오브젝트들 for 돌려서
        {
            if (iter->isActive == false) // 비활성화되어있는애 찾아
                return dynamic_cast<T*>(iter); // 얘 반환
        }
        return nullptr;
    }

    template<class T>
    T* AddObjectAtLayer(LAYERKEY _layerKey, const std::wstring& _objecttag) // 오브젝트를 추가하는 애인가봐
    {
        T* object = new T(); // 오브젝트 생성
        Layer* l = SceneManager::GetInstance()->GetCurrentSceneLayer(_layerKey);// 레이어 가져와
        l->AddGameObject(_objecttag, object); // 그 레이어에 추가해
        return dynamic_cast<T*>(object);
    }

    // 비활성화된 오브젝트를 가져오고 걔를 소환한다 
    template<class T>
    T* CheckActiveFalsedObjectAndSpawn(LAYERKEY _layerKey, const std::wstring& _objecttag)
    {
        T* object = GetActiveFalsedObject<T>(_layerKey, _objecttag); // ㅇㅇ 가져와
        if (object == nullptr) // null이면
        {
            return AddObjectAtLayer<T>(_layerKey, _objecttag); // 만들어
        }
        return object; // 아니면 걍 리턴?
    }
};
END