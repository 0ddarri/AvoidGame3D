#include "DXUT.h"
#include "SaveInfo.h"

void SaveInfo::Initialize(std::string str)
{
	std::cout << "SaveINit" << std::endl;
	path = str;
	Load();
}

void SaveInfo::Load()
{
	std::ifstream read;
	read.open(path);
	if (read.is_open())
	{
		std::string str;
		std::getline(read, str);
		float ptime = atof(str.c_str());
		std::cout << ptime << std::endl;
		maxPlayTime = ptime;
	}
	else std::cout << "LOAD ERROR" << std::endl;
	read.close();
}

void SaveInfo::Save(float playtime)
{
	if (maxPlayTime > playtime) return;
	std::ofstream fwrite;
	fwrite.open(path);
	if (fwrite.is_open())
	{
		std::string buffer(std::to_string(playtime));
		fwrite.write(buffer.c_str(), buffer.size());
		std::wcout << "Save : " << buffer.c_str() << std::endl;
	}
	else std::cout << "SAVE ERROR" << std::endl;
	fwrite.close();
}
