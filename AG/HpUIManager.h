#pragma once
#include "Engine/Singleton.h"
#include "HpUI.h"

class HpUIManager final : public Engine::Singleton<HpUIManager>
{
private:
public:
	std::vector<HpUI*> _hpUIlist;

	void CreateHpUI();
	void SetHpUI();

	void DeleteUI();
};

