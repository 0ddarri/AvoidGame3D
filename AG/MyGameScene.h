#pragma once
#include "Engine/Scene.h"
#include "HpUI.h"

class MyGameScene final : public Engine::Scene
{
private:
protected:
public:
	explicit MyGameScene(void);
	virtual ~MyGameScene(void);

	void Start(void) override;
	void Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Exit(void) override;

	void Free(void) override;

	float _spawnTime = 0.0f;
	float _maxspawnTime = 0.0f;

};

