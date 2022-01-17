#pragma once
#include "Engine/GameObject.h"

NAMESPACE(Engine)
class Transform;
class Collider;
class StaticMesh;
class Shader;
END

class MyMap final : public Engine::GameObject
{
private:
	Engine::StaticMesh* mesh = nullptr;
	Engine::Transform* transform = nullptr;
	Engine::Shader* shader = nullptr;
protected:
public:
	explicit MyMap(void);
	virtual ~MyMap(void);

	void CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject) override;

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;
};

