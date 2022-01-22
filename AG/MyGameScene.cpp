#include "DXUT.h"
#include "Engine/SceneManager.h"
#include "Engine/GraphicsManager.h"
#include "Engine/Layer.h"
#include "Client/CameraManager.h"
#include "MyPlayerCamera.h"
#include "Client/PlaneCamera.h"
#include "MyMap.h"
#include "MyPlayer.h"
#include "Client/SkySphere.h"
#include "PlayerManager.h"
#include "RockManager.h"
#include "Rock.h"
#include "MyGameScene.h"

MyGameScene::MyGameScene(void)
{
}

MyGameScene::~MyGameScene(void)
{
}

void MyGameScene::Start(void)
{
	_spawnTime = 0.0f;
	_maxspawnTime = 1.5f;

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

	PlayerManager::GetInstance()->Initialize();
	Scene::AddGameObject(OBJ2, L"Player", PlayerManager::GetInstance()->GetPlayer());

	//Rock* rock = new Rock();
	//Scene::AddGameObject(OBJ2, L"Rock", rock);

	RockManager::GetInstance()->CreateRock(70);
	for (Rock* rock : RockManager::GetInstance()->_rocklist)
	{
		Scene::AddGameObject(OBJ2, L"Rock", rock);
		std::cout << "Rock Add" << std::endl;
	}
}

void MyGameScene::Update(const FLOAT& dt)
{
	Scene::Update(dt);
	CameraManager::GetInstance()->UpdateCamera(dt);

	_spawnTime += dt;

	if (_spawnTime > _maxspawnTime)
	{
		_maxspawnTime -= 0.01f;
		if (_maxspawnTime < 0.1f) _maxspawnTime = 0.1f;
		_spawnTime = 0.0f;
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_real_distribution<> distr(-1.0f, 1.0f);
		RockManager::GetInstance()->SpawnRock({ (float)distr(eng),5,(float)distr(eng) });
		std::cout << distr(eng) << std::endl;
	}
	RockManager::GetInstance()->DisableRock();
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
