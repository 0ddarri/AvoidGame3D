#include "DXUT.h"
#include "PlayerManager.h"
#include "Engine/GameObject.h"
#include "MyPlayer.h"

void PlayerManager::Initialize()
{
	player = new MyPlayer();
}

Engine::GameObject* PlayerManager::GetPlayer()
{
	return player;
}

D3DXVECTOR3 PlayerManager::GetPlayerPosition()
{
	return player->GetPos();
}

Engine::Transform* PlayerManager::GetPlayerTransform()
{
	return player->GetTransform();
}

D3DXQUATERNION PlayerManager::GetPlayerQuaternion()
{
	return player->GetQuat();
}
