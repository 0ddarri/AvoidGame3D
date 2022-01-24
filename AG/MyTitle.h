#pragma once
#include "Engine/StaticUI.h"

class Trnasform;

class MyTitle final : public Engine::StaticUI
{
private:
protected:
public:
	explicit MyTitle(std::wstring _textureTag);
	virtual ~MyTitle(void);

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;
};

