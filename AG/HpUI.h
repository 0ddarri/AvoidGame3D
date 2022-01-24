#pragma once
#include "Engine/StaticUI.h"

class Trnasform;

class HpUI final : public Engine::StaticUI
{
private:
protected:
public:
	explicit HpUI(std::wstring _textureTag, D3DXVECTOR3 pos);
	virtual ~HpUI();

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;

	bool _visible = true;
};

