#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/ResourceManager.h"
#include "Engine/RenderManager.h"
#include "Engine/StaticMesh.h"
#include "Engine/Collider.h"
#include "Engine/Shader.h"
#include "PlayerManager.h"
#include "Engine/SceneManager.h"
#include "MyPlayer.h"

MyPlayer::MyPlayer(void)
{
	transform = new Engine::Transform();
	componentgroup.emplace(L"Transform", transform);
	mesh = dynamic_cast<Engine::StaticMesh*>(Engine::ResourceManager::GetInstance()->LoadResource(L"Player"));
	componentgroup.emplace(L"Mesh", mesh);
	shader = dynamic_cast<Engine::Shader*>(Engine::ResourceManager::GetInstance()->LoadResource(L"defaultShader"));
	componentgroup.emplace(L"Shader", shader);

	transform->scale = { 0.1,0.1,0.1 };
	transform->localPosition = { 0,0,0 };

	memcpy(&transform->worldMatrix._41, transform->localPosition, sizeof(D3DXVECTOR3));
	std::cout << "Player ������" << std::endl;
	_speed = 10.0f;

	hp = 3;
}

MyPlayer::~MyPlayer(void)
{
}

void MyPlayer::CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject)
{
}

INT MyPlayer::Update(const FLOAT& dt)
{
	D3DXVECTOR3 directionVector = -(*reinterpret_cast<D3DXVECTOR3*>(&transform->worldMatrix._31));
	if (DXUTIsKeyDown('W'))
	{
		transform->localPosition -= directionVector * dt * _speed;
	}
	if (DXUTIsKeyDown('S'))
	{
		transform->localPosition += directionVector * dt * _speed;
	}
	// ���⼭ ��ȭ�� üũ�ϴµ�
	// �̰͵� ���������� ����Ŷ� Ȯ�������� �ʾƿ� ����� ��? ���ϱ� ��ȭ�� �� ������� �ϴ��󱸿� �߽� ��������

	// �׷� ��� �װ� �̿��ؼ� ���ʹϾ� ȸ�� ���ѹ� ���� �װ� �ȵǿ� ������������
	POINT p = { 0,0 };
	POINT Beforep = { 0,0 };
	GetCursorPos(&Beforep);
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	DXUTGetMonitorInfo(DXUTMonitorFromWindow(DXUTGetHWND(), MONITOR_DEFAULTTONEAREST), &mi);
	p.x = (mi.rcMonitor.left + mi.rcMonitor.right) / 2;
	p.y = (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
	SetCursorPos(p.x, p.y);
	PlayerManager::GetInstance()->cursorpos = {(float)Beforep.x - (mi.rcMonitor.left + mi.rcMonitor.right) / 2,(float)Beforep.y - (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2,0.0f};
	transform->Rotate(Engine::Transform::RotType::UP, PlayerManager::GetInstance()->cursorpos.x * dt * 0.1f);

	Engine::RenderManager::GetInstance()->AddRenderObject(ID_NORMALMESH, this);

	//std::cout << transform->localPosition.x << ", " << transform->localPosition.y << ", " << transform->localPosition.z << std::endl;
	if (transform->localPosition.x < -0.7f) transform->localPosition.x = -0.7f;
	if (transform->localPosition.x > 0.7f) transform->localPosition.x = 0.7f;
	if (transform->localPosition.z < -0.7f) transform->localPosition.z = -0.7f;
	if (transform->localPosition.z > 0.7f) transform->localPosition.z = 0.7f;

	GameObject::Update(dt);
	return OBJALIVE;
}

void MyPlayer::LateUpdate(const FLOAT& dt)
{
	GameObject::LateUpdate(dt);
}

void MyPlayer::Render(const FLOAT& dt)
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

void MyPlayer::Free(void)
{
	GameObject::Free();
}

D3DXVECTOR3 MyPlayer::GetPos()
{
	return transform->localPosition;
}

Engine::Transform* MyPlayer::GetTransform()
{
	return transform;
}

D3DXQUATERNION MyPlayer::GetQuat()
{
	return transform->quaternion;
}

bool MyPlayer::CheckDie()
{
	if (hp <= 0) return true;
	return false;
}
