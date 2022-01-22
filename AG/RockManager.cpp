#include "DXUT.h"
#include "Engine/Scene.h"
#include "Engine/SceneManager.h"
#include "Engine/Transform.h"
#include "RockManager.h"

void RockManager::CreateRock(int count)
{
	for (int i = 0; i < count; i++)
	{
		Rock* rock = new Rock();
		rock->_visible = false;
		rock->SetTransform({ 0,0,0 });
		_rocklist.push_back(rock);
	}
}

void RockManager::SpawnRock(D3DXVECTOR3 pos)
{
	for (Rock* rock : _rocklist)
	{
		if (!rock->_visible)
		{
			rock->Initialize();
			rock->_visible = true;
			rock->SetTransform(pos);
			_liverocklist.push_back(rock);
			return;
		}
	}
}

void RockManager::DisableRock()
{
	for (Rock* rock : _liverocklist)
	{
		if (rock->transform->localPosition.y < 0)
		{
			std::cout << "Disable Rock" << std::endl;
			rock->_visible = false;
			rock->SetTransform({ 9999,9999,9999 });
			_liverocklist.remove(rock);
			return;
		}
	}
}

void RockManager::DeleteRock()
{
	for (Rock* rock : _rocklist)
	{
		rock->Free();
	}
	_rocklist.clear();
	_liverocklist.clear();
}
