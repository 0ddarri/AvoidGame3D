#pragma once
#include "GameObject.h"
NAMESPACE(Engine)
class Transform;

class Text final : public GameObject
{
private:
protected:
	LPD3DXFONT font = nullptr;
	std::wstring buffer;
	Transform* transform = nullptr;
public:
	explicit Text(float size, float weight, LPCWSTR fontname, std::wstring buff = L"", D3DXVECTOR3 pos = { 0,0,0 });
	virtual ~Text();

	void SetString(std::wstring str);

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;
};
END