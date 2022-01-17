#include "DXUT.h"
#include "Engine/SceneManager.h"
#include "Engine/GraphicsManager.h"
#include "Engine/Layer.h"
#include "Client/CameraManager.h"
#include "MyPlayerCamera.h"
#include "Client/PlaneCamera.h"
#include "MyMap.h"
#include "Client/SkySphere.h"
#include "MyGameScene.h"

MyGameScene::MyGameScene(void)
{
}

MyGameScene::~MyGameScene(void)
{
}

void MyGameScene::Start(void)
{
	std::cout << "My Game Start" << std::endl;
	Engine::GraphicsManager::GetInstance()->CreateSprite();

	Engine::Layer* l = new Engine::Layer();
	Engine::Layer* l2 = new Engine::Layer();
	Engine::Layer* effect = new Engine::Layer();
	Engine::Layer* Ui = new Engine::Layer();

	layergroup.emplace(OBJ1, l);
	layergroup.emplace(OBJ2, l2);
	layergroup.emplace(EFFECT, effect);
	layergroup.emplace(UI, Ui);


	CameraManager::GetInstance()->AddCamera(CAM_PLAYER, new MyPlayerCamera());
	CameraManager::GetInstance()->SetCamera(CAM_PLAYER);

	SkySphere* skySphere = new SkySphere();
	Scene::AddGameObject(OBJ1, L"skysphere", skySphere);

	MyMap* map = new MyMap();
	Scene::AddGameObject(OBJ1, L"Map", map);
}

void MyGameScene::Update(const FLOAT& dt)
{
	Scene::Update(dt);
	CameraManager::GetInstance()->UpdateCamera(dt);
}

void MyGameScene::LateUpdate(const FLOAT& dt)
{
	Scene::LateUpdate(dt);
	CameraManager::GetInstance()->LateUpdateCamera(dt);
}

void MyGameScene::Exit(void)
{
	Engine::GraphicsManager::GetInstance()->DeleteSprite();
	CameraManager::GetInstance()->DeleteCameraDatas();
	Scene::Exit();
}

void MyGameScene::Free(void)
{
	Scene::Free();
}
