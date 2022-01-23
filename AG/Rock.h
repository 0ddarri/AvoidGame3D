#pragma once
#include "Engine/GameObject.h"

NAMESPACE(Engine)
class Transform;
class StaticMesh;
class Shader;
END

class Rock final : public Engine::GameObject
{
private:
	Engine::StaticMesh* mesh = nullptr;
	Engine::Shader* shader = nullptr;

	float _gravityScale;
	float _velocity;

	float collisionRadius;
public:
	Engine::Transform* transform = nullptr;

	explicit Rock(void);
	virtual ~Rock(void);

	void Initialize();

	void CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject) override;

	bool CheckCol();

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;

	bool _visible;

	void SetTransform(D3DXVECTOR3 pos);

};

