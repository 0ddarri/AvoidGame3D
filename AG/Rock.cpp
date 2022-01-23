#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/ResourceManager.h"
#include "Engine/RenderManager.h"
#include "Engine/CollisionManager.h"
#include "Engine/StaticMesh.h"
#include "Engine/Shader.h"
#include "PlayerManager.h"
#include "Rock.h"

Rock::Rock(void)
{
	transform = new Engine::Transform();
	componentgroup.emplace(L"Transform", transform);
	mesh = dynamic_cast<Engine::StaticMesh*>(Engine::ResourceManager::GetInstance()->LoadResource(L"Rock"));
	componentgroup.emplace(L"Mesh", mesh);
	shader = dynamic_cast<Engine::Shader*>(Engine::ResourceManager::GetInstance()->LoadResource(L"defaultShader"));
	componentgroup.emplace(L"Shader", shader);

	transform->scale = { 0.3,0.3,0.3 };
	transform->localPosition = { 9999,9999,9999 };
	memcpy(&transform->worldMatrix._41, transform->localPosition, sizeof(D3DXVECTOR3));

	_gravityScale = 1;
	_visible = false;

	collisionRadius = 0.1f;
}

Rock::~Rock(void)
{
}

void Rock::Initialize()
{
	_visible = false;
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<> distr(0.1f, 0.3f);
	transform->scale = { (float)distr(eng),(float)distr(eng),(float)distr(eng) };
}

void Rock::CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject)
{

}

bool Rock::CheckCol()
{
	if (D3DXVec3Length(&(transform->localPosition - PlayerManager::GetInstance()->GetPlayerPosition())) < collisionRadius)
	{
		std::cout << "Ãæµ¹" << std::endl;
		PlayerManager::GetInstance()->player->hp--;
		std::cout << PlayerManager::GetInstance()->player->hp << std::endl;
		return true;
	}
	return false;
}

INT Rock::Update(const FLOAT& dt)
{
	if (_visible)
	{
		CheckCol();
		Engine::RenderManager::GetInstance()->AddRenderObject(ID_NORMALMESH, this);
		transform->localPosition.y -= _gravityScale * dt;
	}


	GameObject::Update(dt);
	return OBJALIVE;
}

void Rock::LateUpdate(const FLOAT& dt)
{
	GameObject::LateUpdate(dt);
}

void Rock::Render(const FLOAT& dt)
{
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	shader->SetupTable(transform->worldMatrix);
	UINT pass = 0;
	LPD3DXEFFECT tempeffect = shader->GetEffect();
	tempeffect->Begin(&pass, 0);
	tempeffect->BeginPass(0);
	mesh->RenderMesh(tempeffect);
	tempeffect->EndPass();
	tempeffect->End();
	//collider->RenderCollider();
	GameObject::Render(dt);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Rock::Free(void)
{
	GameObject::Free();
}

void Rock::SetTransform(D3DXVECTOR3 pos)
{
	transform->localPosition = pos;
}
