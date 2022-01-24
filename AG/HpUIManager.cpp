#include "DXUT.h"
#include "PlayerManager.h"
#include "HpUIManager.h"

void HpUIManager::CreateHpUI()
{
	for (int i = 0; i < 5; i++)
	{
		HpUI* _hp = new HpUI(L"Heart", D3DXVECTOR3(100 * (i + 1), 100, 0));
		_hp->_visible = true;
		_hpUIlist.emplace_back(_hp);
	}
}

void HpUIManager::SetHpUI()
{
	for (int i = 0; i < 5; i++)
	{
		int hp = PlayerManager::GetInstance()->player->hp;
		if (i < hp)
		{
			_hpUIlist[i]->_visible = true;
		}
		else
		{
			_hpUIlist[i]->_visible = false;
		}
	}
}

void HpUIManager::DeleteUI()
{
	_hpUIlist.clear();
}
