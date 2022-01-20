#pragma once
#include "Engine/GameObject.h"

NAMESPACE(Engine)
class Transform;
class Collider;
class StaticMesh;
class Shader;
END

class MyPlayer final : public Engine::GameObject
{
private:
	Engine::StaticMesh* mesh = nullptr;
	Engine::Transform* transform = nullptr;
	Engine::Shader* shader = nullptr;
protected:
public:
	explicit MyPlayer(void);
	virtual ~MyPlayer(void);

	void CollisionEvent(const std::wstring& _objectTag, GameObject* _gameObject) override;

	INT Update(const FLOAT& dt) override;
	void LateUpdate(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	void Free(void) override;

	D3DXVECTOR3 GetPos();
	Engine::Transform* GetTransform();
	D3DXQUATERNION GetQuat();

	float _speed;
};

