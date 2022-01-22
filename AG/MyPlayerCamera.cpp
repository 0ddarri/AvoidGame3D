#include "DXUT.h"
#include "Engine/Transform.h"
#include "Engine/Camera.h"
#include "PlayerManager.h"
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
	FOV = 80.0f;
	transform = new Engine::Transform();
	_camera = new Engine::Camera();
	componentgroup.emplace(L"Transform", transform);
	componentgroup.emplace(L"Camera", _camera);

	transform->curQuaternion = D3DXQUATERNION(0, 1, 0, 0);

	_camera->SetProjectionMatrix(D3DXToRadian(FOV), // FOV네?
		static_cast<float>(DXUTGetD3D9BackBufferSurfaceDesc()->Width) /
		static_cast<float>(DXUTGetD3D9BackBufferSurfaceDesc()->Height),
		0.01, 50000);
	transform->localPosition = { 0,0.5,0 };
}

INT MyPlayerCamera::Update(const FLOAT& dt)
{
	//[]  []
	//
	//[]  []

	//D3DXMATRIX wm = DEVICE->GetTransform(D3DTS_WORLD);

	D3DXVECTOR3 directionVector = -(*reinterpret_cast<D3DXVECTOR3*>(&transform->worldMatrix._31)); // 이거가 디버그 찍어보면 바라보는방향 나오는거같던데용
	//D3DXVECTOR3 vec = *reinterpret_cast<D3DXVECTOR3*>(&transform->worldMatrix.m[0][0]); // 이거가 느낌상 위아래 회전할때 쓴느거같아서
	////썼어요
	//vec.y = 0.0f;
	//transform = PlayerManager::GetInstance()->GetPlayerTransform();
	D3DXVECTOR3 pos = PlayerManager::GetInstance()->GetPlayerPosition();
	transform->localPosition = { pos.x,pos.y + 0.4f,pos.z };
	GameObject::Update(dt);
	//transform->RotateY(Engine::Transform::RotType::RIGHT , PlayerManager::GetInstance()->cursorpos.y * dt * 0.1f);
	//transform->RotateAxis(vec, PlayerManager::GetInstance()->cursorpos.y * dt * 0.1f);


	//D3DXVECTOR3 up = D3DXVECTOR3(0,1,0);
	//D3DXVECTOR3 cross = *D3DXVec3Cross(&cross, &directionVector, &up);
	//D3DXVec3Normalize(&cross, &cross);

	xrot += PlayerManager::GetInstance()->cursorpos.y* dt * 0.1f;
	yrot += PlayerManager::GetInstance()->cursorpos.x* dt * 0.1f;
	if (xrot < -1.3f) xrot = -1.3f;
	if (xrot > 1.3f) xrot = 1.3f;

	D3DXMATRIX matRotX = *D3DXMatrixRotationX(&matRotX, xrot);
	D3DXMATRIX matRotY = *D3DXMatrixRotationY(&matRotY, yrot);

	D3DXMATRIX matRot = matRotX * matRotY;

	D3DXQUATERNION q = *D3DXQuaternionRotationMatrix(&q, &matRot);
	transform->quaternion = q; // 둘이 반전됬어용 

	return OBJALIVE;
}

void MyPlayerCamera::LateUpdate(const FLOAT& dt)
{
	D3DXMATRIX matView;
	D3DXMatrixInverse(&matView, 0, &transform->worldMatrix);

	_camera->SetViewMatrix(matView);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	worldCameraPosition = D3DXVECTOR4(transform->localPosition, 1);
	_camera->Render(dt);
	GameObject::LateUpdate(dt);
}

void MyPlayerCamera::Render(const FLOAT& dt)
{
	GameObject::Render(dt);
}

void MyPlayerCamera::Free(void)
{
}
