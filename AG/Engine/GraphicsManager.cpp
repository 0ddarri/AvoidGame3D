#include "DXUT.h"
#include "GraphicsManager.h"
USING(Engine)

// 스프라이트 만들어
void GraphicsManager::CreateSprite(void)
{
	D3DXCreateSprite(DEVICE, &sprite);
}

// ResetDevice할 때 호출하는앤가
void GraphicsManager::ResetDevice(void)
{
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->LightEnable(0, false);	
	if(sprite != nullptr)
		sprite->OnResetDevice();
}

// LostDevice할 때 호출하는앤가
void GraphicsManager::LostDevice(void)
{
	if (sprite != nullptr)
		sprite->OnLostDevice();
}

void GraphicsManager::DeleteSprite(void)
{
	if (sprite != nullptr)
		sprite->Release();
}