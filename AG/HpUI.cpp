#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/RenderManager.h"
#include "HpUI.h"

HpUI::HpUI(std::wstring _textureTag, D3DXVECTOR3 pos) : Engine::StaticUI(_textureTag)
{
	transform->localPosition = pos;
	transform->scale = { 0.3,0.3,0.3 };
}

HpUI::~HpUI()
{

}

INT HpUI::Update(const FLOAT& dt)
{
	Engine::StaticUI::Update(dt);
	if (_visible)
	{
		Engine::RenderManager::GetInstance()->AddRenderObject(ID_UI, this);
	}
	return 0;
}

void HpUI::LateUpdate(const FLOAT& dt)
{
	Engine::StaticUI::LateUpdate(dt);
}

void HpUI::Render(const FLOAT& dt)
{
	Engine::StaticUI::Render(dt);
}

void HpUI::Free(void)
{
}
