#pragma once
#include "Engine/Singleton.h"
#include "MyPlayer.h"


NAMESPACE(Engine)
class GameObject;
class Transform;
END

class PlayerManager : public Engine::Singleton<PlayerManager>
{
private:
protected:
public:
	MyPlayer* player;

	void Initialize();

	Engine::GameObject* GetPlayer();

	D3DXVECTOR3 GetPlayerPosition();

	Engine::Transform* GetPlayerTransform();
	D3DXQUATERNION GetPlayerQuaternion();

	D3DXVECTOR3 cursorpos = { 0,0,0 };
};

