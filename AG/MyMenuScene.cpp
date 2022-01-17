#include "DXUT.h"
#include "Engine/Button.h"
#include "Engine/Layer.h"
#include "Engine/SceneManager.h"
#include "Engine/GraphicsManager.h"
#include "Client/CameraManager.h"
#include "Client/MenuCamera.h"
#include "MyTitle.h"
#include "MyMenuScene.h"

MyMenuScene::MyMenuScene(void)
{

}

MyMenuScene::~MyMenuScene(void)
{
}

void MyMenuScene::Start(void)
{
	Engine::GraphicsManager::GetInstance()->CreateSprite();

	std::cout << "My Menu Start" << std::endl;
	Engine::Layer* l = new Engine::Layer();
	layergroup.emplace(UI, l);

	_button = new Engine::Button(L"Start", { 0,0,5 });
	_button->SetButtonFunction([]() {Engine::SceneManager::GetInstance()->SetScene(L"°ÔÀÓ"); });
	l->AddGameObject(L"Objects", _button);

	_title = new MyTitle(L"Title");
	l->AddGameObject(L"Title", _title);

	CameraManager::GetInstance()->AddCamera(CAM_MENU, new MenuCamera());
	CameraManager::GetInstance()->SetCamera(CAM_MENU);
}

void MyMenuScene::Update(const FLOAT& dt)
{
	if (DXUTIsMouseButtonDown(VK_LBUTTON))
	{
		if (_button->IsMouseOn())
		{
			_button->GetButtonFunction()();
		}
	}

	Scene::Update(dt);
	CameraManager::GetInstance()->UpdateCamera(dt);
}

void MyMenuScene::LateUpdate(const FLOAT& dt)
{
	Scene::LateUpdate(dt);
	CameraManager::GetInstance()->LateUpdateCamera(dt);
}

void MyMenuScene::Exit(void)
{
	CameraManager::GetInstance()->DeleteCameraDatas();
	Scene::Exit();
}

void MyMenuScene::Free(void)
{
	Scene::Free();
}
