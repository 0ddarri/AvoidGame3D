#pragma once
#include "Engine/Singleton.h"

class MyPlayer;

NAMESPACE(Engine)
class GameObject;
class Transform;
END

class PlayerManager final : public Engine::Singleton<PlayerManager>
{
private:
	MyPlayer* player;
protected:
public:
	void Initialize();

	Engine::GameObject* GetPlayer();

	D3DXVECTOR3 GetPlayerPosition();

	Engine::Transform* GetPlayerTransform();
	D3DXQUATERNION GetPlayerQuaternion();

	D3DXVECTOR3 cursorpos = { 0,0,0 };
};

