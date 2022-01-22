#pragma once
#include "Engine/Singleton.h"
#include "Rock.h"
class RockManager final : public Engine::Singleton<RockManager>
{
private:
public:
	std::list<Rock*> _rocklist;
	std::list<Rock*> _liverocklist;
	void CreateRock(int count);
	void SpawnRock(D3DXVECTOR3 pos);
	void DisableRock();
	void DeleteRock();
};