#include "DXUT.h"
#include "GameObject.h"
#include "Layer.h"

USING(Engine)

void Layer::AddGameObject(const std::wstring& _objectKey, GameObject* _object) // 대충 특정 레이어에 옵젝 추가하는건가
{
	if (_object == nullptr) // object null이면 컷
		return;
	
	const auto& iter = gameobjectgroup.find(_objectKey); // const 붙이면 안전하고 빠름(상수참조) 무슨 복사를 안해서 더 빠르대
	if (iter == gameobjectgroup.end()) // 처음부터 끝까지 뒤졌는데 못찾으면
		gameobjectgroup[_objectKey] = std::list<GameObject*>(); // ?? 리스트가 없으니까 초기화를 해준다

	gameobjectgroup[_objectKey].push_back(_object); // group의 _objectKey번째의 list에 넣음
}

void Layer::Update(const FLOAT& dt)
{
	for (auto& objectlist : gameobjectgroup) // 
	{
		auto& iter = objectlist.second.begin(); // gameobjectlist 처음
		const auto& iter_end = objectlist.second.end(); // 끝
		for (; iter != iter_end;) // while 느낌인가? iter랑 end가 다르면 계속 도는느낌인거같은데
		{
			if ((*iter)->Update(dt) == OBJDEAD) // 옵젝이 뒤졌으면?
			{
				Safe_Release(*iter); // 해제
				iter = objectlist.second.erase(iter); // 리스트에서 제거
			}
			else // 잘 살아있으면?
				++iter; // 다음껄로?
		}
	}
}

void Layer::LateUpdate(const FLOAT& dt) // Update 함수 호출되고 이게 호출되는건가
{
	for (auto& objectlist : gameobjectgroup) // gameobjectgroup 돌아
	{
		for (auto& object : objectlist.second) // 그 안에 list 돌아
		{
			if (object->GetActive() == true) // 옵젝 켜져있으면
				object->LateUpdate(dt); // LateUpdate 실행
		}
	}
}

void Layer::ResetDevice(void) // 디바이스 리셋되었을때?
{
	for (auto& objectlist : gameobjectgroup)
	{
		for (auto& object : objectlist.second)
		{
			object->ResetDevice();
		}
	}
}

void Layer::LostDevice(void) // 디바이스 잃어버렸을때?
{
	for (auto& objectlist : gameobjectgroup)
	{
		for (auto& object : objectlist.second)
		{
			object->LostDevice();
		}
	}
}

void Layer::Free(void)
{
	for (auto& objectlist : gameobjectgroup)
	{
		for (auto& object : objectlist.second)
		{
			Safe_Release(object);
		}
		objectlist.second.clear();
	}
	gameobjectgroup.clear();
}
