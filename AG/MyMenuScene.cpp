#include "DXUT.h"
#include "Engine/Button.h"
#include "Engine/Layer.h"
#include "Engine/SceneManager.h"
#include "Engine/Sound.h"
#include "Engine/GraphicsManager.h"
#include "Client/CameraManager.h"
#include "Client/MenuCamera.h"
#include "MyTitle.h"
#include "SaveInfo.h"
#include "MyMenuScene.h"

MyMenuScene::MyMenuScene(void)
{

}

MyMenuScene::~MyMenuScene(void)
{
}

void MyMenuScene::Start(void)
{

	SaveInfo::GetInstance()->Initialize("info.txt");

	Engine::GraphicsManager::GetInstance()->CreateSprite();

	std::cout << "My Menu Start111111111111" << std::endl;
	Engine::Layer* l = new Engine::Layer();
	layergroup.emplace(UI, l);

	_button = new Engine::Button(L"Start", { 0,0,5 });
	_button->SetButtonFunction([]() {Engine::SceneManager::GetInstance()->SetScene(L"°ÔÀÓ"); });
	l->AddGameObject(L"Objects", _button);

	_title = new MyTitle(L"Title");
	l->AddGameObject(L"Title", _title);

	CameraManager::GetInstance()->AddCamera(CAM_MENU, new MenuCamera());
	CameraManager::GetInstance()->SetCamera(CAM_MENU);

	text = new Engine::Text(2, 500, L"Arial", L"SSS", { 100,300,0 });
	l->AddGameObject(L"Text", text);
	std::wstring str = L"Max PlayTime : ";
	std::wstring buffer(std::to_wstring(SaveInfo::GetInstance()->maxPlayTime));
	std::wstring st = str + buffer;
	text->SetString(st);

	Engine::Sound::GetInstance()->SoundStop(L"BGM", true);
	Engine::Sound::GetInstance()->SoundPlay(L"BGM", true, volume);
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

	if (DXUTIsKeyDown(VK_F1))
	{
		volume += dt * 1000;
		std::cout << volume << std::endl;
		Engine::Sound::GetInstance()->SoundStop(L"BGM", false);
		Engine::Sound::GetInstance()->SoundPlay(L"BGM", true, volume);
	}
	if (DXUTIsKeyDown(VK_F2))
	{
		volume -= dt * 1000;
		std::cout << volume << std::endl;
		Engine::Sound::GetInstance()->SoundStop(L"BGM", false);
		Engine::Sound::GetInstance()->SoundPlay(L"BGM", true, volume);
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
	Engine::GraphicsManager::GetInstance()->DeleteSprite();
	Scene::Exit();
}

void MyMenuScene::Free(void)
{
	Scene::Free();
}
