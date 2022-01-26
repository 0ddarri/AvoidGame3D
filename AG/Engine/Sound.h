#pragma once
#include "Singleton.h"

NAMESPACE(Engine)
class Sound final : public Singleton<Sound>
{
private:
CSoundManager* soundManager = nullptr;
std::map<std::wstring, CSound*> soundlist;
public:
	void Initialize();

	void LoadSound();

	void Load(LPWSTR path, std::wstring name);

	void SoundPlay(std::wstring name, bool loop = false, LONG volume = 0);
	void SoundStop(std::wstring name, bool reset = false);
};
END