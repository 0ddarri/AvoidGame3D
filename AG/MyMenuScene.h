#pragma once
#include "Engine/Scene.h"
#include "Engine/Text.h"
NAMESPACE(engine)
class Button;
END

class MyTitle;

class MyMenuScene final : public Engine::Scene
{
protected:
	Engine::Button* _button;
	MyTitle* _title;
	Engine::Text* text;
public:
	explicit MyMenuScene(void);
	virtual ~MyMenuScene(void);

	void Start(void) override;
	void Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Exit(void) override;

	void Free(void) override;

	float volume = -1000.0f;
};

