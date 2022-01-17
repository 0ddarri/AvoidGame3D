#pragma once
#include "Base.h"

NAMESPACE(Engine)
class GameObject;
class Layer final : public Base // final : 상속을 막는다
{
private:
protected:
public:
	explicit Layer(void) {};
	virtual ~Layer(void) {};

	std::unordered_map<std::wstring, std::list<GameObject*>> gameobjectgroup; // hash 자료구조, 검색이 빠름

	void AddGameObject(const std::wstring& _objectKey, GameObject* _object);

	void Update(const FLOAT& dt);
	void LateUpdate(const FLOAT& dt);

	void ResetDevice(void);
	void LostDevice(void);

	void Free(void) override;
};
END
