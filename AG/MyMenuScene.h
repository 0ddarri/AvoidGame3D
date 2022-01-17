#pragma once
#include "Engine/Scene.h"
NAMESPACE(engine)
class Button;
END

class MyTitle;

class MyMenuScene final : public Engine::Scene
{
protected:
	Engine::Button* _button;
	MyTitle* _title;
public:
	explicit MyMenuScene(void);
	virtual ~MyMenuScene(void);

	void Start(void) override;
	void Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Exit(void) override;

	void Free(void) override;
};

