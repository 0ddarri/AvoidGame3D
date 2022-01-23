#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/ResourceManager.h"
#include "Engine/RenderManager.h"
#include "Engine/StaticMesh.h"
#include "Engine/Shader.h"
#include "MyMap.h"

MyMap::MyMap(void)
{
	transform = new Engine::Transform();
	componentgroup.emplace(L"Transform", transform);
	mesh = dynamic_cast<Engine::StaticMesh*>(Engine::ResourceManager::GetInstance()->LoadResource(L"Map"));
	componentgroup.emplace(L"Mesh", mesh);
	shader = dynamic_cast<Engine::Shader*>(Engine::ResourceManager::GetInstance()->LoadResource(L"defaultShader"));
	componentgroup.emplace(L"Shader", shader);

	transform->scale = { 1,1,1 };
	transform->localPosition = D3DXVECTOR3(0, 0, 0);
	std::cout << "Map »ý¼ºÀÚ" << std::endl;

	memcpy(&transform->worldMatrix._41, transform->localPosition, sizeof(D3DXVECTOR3));
}

MyMap::~MyMap(void)
{
}

void MyMap::CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject)
{
}

INT MyMap::Update(const FLOAT& dt)
{

	Engine::RenderManager::GetInstance()->AddRenderObject(ID_NORMALMESH, this);
	GameObject::Update(dt);
	return OBJALIVE;
}

void MyMap::LateUpdate(const FLOAT& dt)
{
	GameObject::LateUpdate(dt);
}

void MyMap::Render(const FLOAT& dt)
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

void MyMap::Free(void)
{
	GameObject::Free();
}
