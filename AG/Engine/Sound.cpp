#include "DXUT.h"
#include "Sound.h"
USING(Engine)

void Sound::Initialize()
{
	soundManager = new CSoundManager;
	soundManager->Initialize(DXUTGetHWND(), DSSCL_PRIORITY);
	soundManager->SetPrimaryBufferFormat(2, 22050, 16);
	std::cout << "SoundInit" << std::endl;
}

void Sound::LoadSound()
{
	Load(L"Resources/Sounds/bg.wav", L"BGM");
	Load(L"Resources/Sounds/hit.wav", L"HIT");
}

void Sound::Load(LPWSTR path, std::wstring name)
{
	CSound* sound;
	auto create = soundManager->Create(&sound, path, DSBCAPS_CTRLVOLUME, GUID_NULL, 15);
	if (FAILED(create))
	{
		std::wcout << "SOUND FAIL : " << path << std::endl;
		delete sound;
		return;
	}
	std::wcout << "SOUND LOAD : " << path << std::endl;
	soundlist.emplace(name, sound);
}

void Sound::SoundPlay(std::wstring name, bool loop, LONG volume)
{
	auto find = soundlist.find(name);
	if (find != soundlist.end())
	{
		std::cout << "Play Sound" << std::endl;
		find->second->Play(0, loop, volume);
	}
}

void Sound::SoundStop(std::wstring name, bool reset)
{
	auto find = soundlist.find(name);
	if (find != soundlist.end())
	{
		find->second->Stop();
		if (reset) find->second->Reset();
	}
}
