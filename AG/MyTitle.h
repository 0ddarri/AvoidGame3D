#pragma once
#include "Engine/StaticUI.h"

class PlayerObserver;
class Trnasform;

class MyTitle final : public Engine::StaticUI
{
private:
	PlayerObserver* observer = nullptr;
protected:
public:
	explicit MyTitle(std::wstring _textureTag);
	virtual ~MyTitle(void);

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;
};

