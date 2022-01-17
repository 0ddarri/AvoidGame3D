#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/Camera.h"
#include "MyPlayerCamera.h"

MyPlayerCamera::MyPlayerCamera(void)
{
	Initialize();
}

MyPlayerCamera::~MyPlayerCamera(void)
{
}

void MyPlayerCamera::Initialize(void)
{
	transform = new Engine::Transform();
	targetTransform = new Engine::Transform();
	_camera = new Engine::Camera();
	componentgroup.emplace(L"Transform", transform);
	componentgroup.emplace(L"targetTransform", targetTransform);
	componentgroup.emplace(L"Camera", _camera);

	targetTransform->localPosition = { 0, 250, 0 };
	transform->curQuaternion = D3DXQUATERNION(0, 1, 0, 0);

	_camera->SetProjectionMatrix(D3DXToRadian(100), // FOV³×?
		static_cast<float>(DXUTGetD3D9BackBufferSurfaceDesc()->Width) /
		static_cast<float>(DXUTGetD3D9BackBufferSurfaceDesc()->Height),
		0.01, 50000);
}

INT MyPlayerCamera::Update(const FLOAT& dt)
{
	transform->localPosition.x += dt * 1090;
	GameObject::Update(dt);
	return OBJALIVE;
}

void MyPlayerCamera::LateUpdate(const FLOAT& dt)
{
	GameObject::LateUpdate(dt);
}

void MyPlayerCamera::Render(const FLOAT& dt)
{
	GameObject::Render(dt);
}

void MyPlayerCamera::Free(void)
{
}
