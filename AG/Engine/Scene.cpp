#include "DXUT.h"
#include "Layer.h"
#include "RenderManager.h"
#include "Scene.h"

USING(Engine)

void Scene::AddGameObject(LAYERKEY _layerKey, std::wstring _objecttag, GameObject* _object)
{
	auto& const iter = layergroup.find(_layerKey); // ���̾�Ű ã��
	if (iter != layergroup.end()) // ������
		layergroup[_layerKey]->AddGameObject(_objecttag, _object); // �� ���̾ ���� �߰�
	else // ������
	{
		Layer* l = new Layer(); // ���̾� �����
		layergroup.emplace(_layerKey, l); // �׷쿡 �־�
		l->AddGameObject(_objecttag, _object); // �� ���̾ �����߰�
		l = nullptr; // ����
	}
}

// ���̾� �޾ƿ���
Layer* Scene::GetLayer(LAYERKEY _layerKey)
{
	auto& const iter = layergroup.find(_layerKey);
	if (iter != layergroup.end())
		return layergroup[_layerKey];
	return nullptr;
}

// ���̾� �׷���� ������ update
void Scene::Update(const FLOAT& dt)
{
	for (auto& const layer : layergroup)
	{
		layer.second->Update(dt);
	}
}
// ���̾� �׷���� ������ lateupdate
void Scene::LateUpdate(const FLOAT& dt)
{
	for (auto& const layer : layergroup)
	{
		layer.second->LateUpdate(dt);
	}
}
// ���̾� �׷���� ������ release
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
