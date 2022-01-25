#include "DXUT.h"
#include "RenderManager.h"
#include "Transform.h"
#include "Text.h"
USING(Engine)

Text::Text(float size, float weight, LPCWSTR fontname, std::wstring buff, D3DXVECTOR3 pos)
{
	transform = new Transform();
	componentgroup.emplace(L"Trasnform", transform);

	SetString(buff);
	transform->localPosition = pos;
	D3DXCreateFont(DEVICE, 20 * size, 0, weight, 1, 0, 1, 0, 0, 0 | FF_DONTCARE, fontname, &font);
	std::cout << "Text" << std::endl;
}

Text::~Text()
{
}

void Text::SetString(std::wstring str)
{
	buffer = str;
}

INT Text::Update(const FLOAT& dt)
{
	RenderManager::GetInstance()->AddRenderObject(ID_UI, this);
 	GameObject::Update(dt);
	return OBJALIVE;
}

void Text::LateUpdate(const FLOAT& dt)
{
	GameObject::LateUpdate(dt);
}

void Text::Render(const FLOAT& dt)
{
	RECT r;
	SetRect(&r, (long)transform->localPosition.x, (long)transform->localPosition.y, 0, 0);
	font->DrawTextW(nullptr, buffer.c_str(), -1, &r, DT_NOCLIP, D3DXCOLOR(1,1,1,1));
	GameObject::Render(dt);
}

void Text::Free(void)
{
	GameObject::Free();
}
