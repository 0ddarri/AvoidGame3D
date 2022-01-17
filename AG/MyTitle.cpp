#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/RenderManager.h"
#include "Engine/SubjectManager.h"
#include "Client/PlayerObserver.h"
#include "MyTitle.h"

MyTitle::MyTitle(std::wstring _textureTag) : Engine::StaticUI(_textureTag)
{
	std::cout << "tlqkf" << std::endl;
	transform->localPosition.x = windowWidth / 2;
	transform->localPosition.y = 100;
	transform->scale = { 0.6f,0.6f,0.6f };
}

MyTitle::~MyTitle(void)
{
}

INT MyTitle::Update(const FLOAT& dt)
{
	Engine::StaticUI::Update(dt);
	Engine::RenderManager::GetInstance()->AddRenderObject(ID_UI, this);
	return OBJALIVE;
}

void MyTitle::LateUpdate(const FLOAT& dt)
{
	Engine::StaticUI::LateUpdate(dt);
}

void MyTitle::Render(const FLOAT& dt)
{
	Engine::StaticUI::Render(dt);
}

void MyTitle::Free(void)
{
	Engine::StaticUI::Free();
}
