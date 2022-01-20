#pragma once
#include "Engine/GameObject.h"

NAMESPACE(Engine)
class Transform;
class Camera;
END

class MyPlayerCamera final : public Engine::GameObject
{
private:
	Engine::Transform* transform = nullptr;
	Engine::Camera* _camera = nullptr;

	float xrot = 0;
	float yrot = 0;
protected:
public:
	explicit MyPlayerCamera(void);
	virtual ~MyPlayerCamera(void);

	void Initialize(void);
	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;
	void Free(void) override;

	float FOV;
};

