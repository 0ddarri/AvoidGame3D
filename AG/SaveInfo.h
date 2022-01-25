#pragma once
#include "Engine/Singleton.h"
class SaveInfo final : public Engine::Singleton<SaveInfo>
{
private:
	std::string path;
public:
	void Initialize(std::string str);
	void Save(float playtime);
	void Load();

	float maxPlayTime;
};

