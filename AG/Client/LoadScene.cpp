#include "DXUT.h"
#include "../Engine/Sound.h"
#include "../Engine/ResourceManager.h"
#include "../Engine/SceneManager.h"
#include "LoadScene.h"

LoadScene::LoadScene(void)
{
}

LoadScene::~LoadScene(void)
{
}

void LoadScene::Start(void)
{
	Engine::Sound::GetInstance()->LoadSound();

	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"skysphere.X", L"SkySphere");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::DYNAMIC, L"Resources/Meshes/Dynamic/", L"Plane.X", L"dynamic");
	Engine::ResourceManager::GetInstance()->AddShader(L"Resources/Shaders/DynamicMeshShader.fx", L"dyshader");
	Engine::ResourceManager::GetInstance()->AddShader(L"Resources/Shaders/DefaultShader.fx", L"defaultShader");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/AimPoint.png", L"AimPoint");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/heart.png", L"Heart");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/trail.dds", L"TrailTexture");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::DYNAMIC, L"Resources/Meshes/Dynamic/", L"Monster1.X", L"Monster1");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"cloud.X", L"Cloud");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"missilepivottest.X", L"Missile");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/muzzleflash (%d).png", L"muzzleFlash", 3);
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Lock.png", L"LockPoint", 1);
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"MonsterBullet.X", L"MonsterBullet");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Start.png", L"Start", 1);
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Title.png", L"Title", 1); // 타이틀 스프라이트 추가
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"map1.X", L"Map");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"player2222.X", L"Player");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"rock.X", L"Rock");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Select.png", L"Select", 1);
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Settings.png", L"Settings", 1);
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Exit.png", L"Exit", 1);
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::DYNAMIC, L"Resources/Meshes/Dynamic/", L"Monster2.X", L"Monster2");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/hit (%d).png", L"Hit", 3);
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::STATIC, L"Resources/Meshes/Static/", L"seatest.X", L"Sea");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/shadow.png", L"Shadow", 1);
	Engine::ResourceManager::GetInstance()->AddShader(L"Resources/Shaders/ShadowShader.fx", L"ShadowShader");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/explosion (%d).png", L"Explosion", 4);
	Engine::ResourceManager::GetInstance()->AddShader(L"Resources/Shaders/SeaShader.fx", L"SeaShader");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::DYNAMIC, L"Resources/Meshes/Dynamic/", L"Boss.X", L"Boss");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::DYNAMIC, L"Resources/Meshes/Dynamic/", L"BossDashGun.X", L"BossDashGun");
	Engine::ResourceManager::GetInstance()->AddMesh(MeshType::DYNAMIC, L"Resources/Meshes/Dynamic/", L"BossShootGun.X", L"BossShootGun");
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/Radar.png", L"RadarPlane", 1);
	Engine::ResourceManager::GetInstance()->AddTexture(L"Resources/Textures/RadarDot.png", L"RadarDot", 1);

	Engine::SceneManager::GetInstance()->SetScene(L"메뉴");

}

void LoadScene::Update(const FLOAT& dt)
{
	Scene::Update(dt);
}

void LoadScene::LateUpdate(const FLOAT& dt)
{
	Scene::LateUpdate(dt);
}

void LoadScene::Exit(void)
{
	Scene::Exit();
}

void LoadScene::Free(void)
{
	Scene::Free();
}
