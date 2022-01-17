#include "DXUT.h"
#include "Layer.h"
#include "RenderManager.h"
#include "Scene.h"

USING(Engine)

void Scene::AddGameObject(LAYERKEY _layerKey, std::wstring _objecttag, GameObject* _object)
{
	auto& const iter = layergroup.find(_layerKey); // 레이어키 찾아
	if (iter != layergroup.end()) // 있으면
		layergroup[_layerKey]->AddGameObject(_objecttag, _object); // 그 레이어에 옵젝 추가
	else // 없으면
	{
		Layer* l = new Layer(); // 레이어 만들어
		layergroup.emplace(_layerKey, l); // 그룹에 넣어
		l->AddGameObject(_objecttag, _object); // 그 레이어에 옵젝추가
		l = nullptr; // 제거
	}
}

// 레이어 받아오기
Layer* Scene::GetLayer(LAYERKEY _layerKey)
{
	auto& const iter = layergroup.find(_layerKey);
	if (iter != layergroup.end())
		return layergroup[_layerKey];
	return nullptr;
}

// 레이어 그룹들의 옵젝들 update
void Scene::Update(const FLOAT& dt)
{
	for (auto& const layer : layergroup)
	{
		layer.second->Update(dt);
	}
}
// 레이어 그룹들의 옵젝들 lateupdate
void Scene::LateUpdate(const FLOAT& dt)
{
	for (auto& const layer : layergroup)
	{
		layer.second->LateUpdate(dt);
	}
}
// 레이어 그룹들의 옵젝들 release
void Scene::Exit(void)
{
	for (auto& const layer : layergroup)
	{
		if (layer.second != nullptr)
		{
			Safe_Release(layer.second);
		}
	}
	layergroup.clear();

}

void Scene::ResetDevice(void)
{
	for (auto& const layer : layergroup)
	{
		layer.second->ResetDevice();
	}
}

void Scene::LostDevice(void)
{
	for (auto& const layer : layergroup)
	{
		layer.second->LostDevice();
	}
}

void Scene::Free(void)
{
	for (auto& const layer : layergroup)
	{
		if (layer.second != nullptr)
		{
			Safe_Release(layer.second);
		}
	}
	layergroup.clear();
}
