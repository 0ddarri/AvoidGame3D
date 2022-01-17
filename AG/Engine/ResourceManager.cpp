#include "DXUT.h"
#include "Resources.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"
USING(Engine)
HRESULT ResourceManager::AddTexture(std::wstring _filepath, std::wstring _tag, const UINT& _texturecount)
{
	auto& iter = resourcegroup.find(_tag); // resourcegroup에서 tag 찾아
	if (iter != resourcegroup.end()) // 있으면
	{
		std::wstring error = _tag; // 
		MessageBox(DXUTGetHWND(), error.c_str(), L"중복 텍스쳐", MB_ICONERROR | MB_OK); // 중복된 텍스처라고 머라함
		return E_FAIL;
	}
	
	Texture* texture = new Texture(); // 없으면 일로와서 텍스처 새로 만들어
	texture->LoadTexture(_filepath, _texturecount); // 로딩해
	resourcegroup.emplace(_tag, texture); // 넣어
	return S_OK;
}

HRESULT ResourceManager::AddMesh(MeshType _meshtype, std::wstring _filepath, std::wstring _filename, std::wstring _tag)
{
	auto& iter = resourcegroup.find(_tag); // 리스트에서 찾아
	if (iter != resourcegroup.end()) // 있으면 
	{
		std::wstring error = _tag;
		MessageBox(DXUTGetHWND(), error.c_str(), L"중복 메시", MB_ICONERROR | MB_OK); // 머라해
		return E_FAIL;
	}

	if (_meshtype == MeshType::STATIC) // 스태틱메쉬
	{
		StaticMesh* staticmesh = new StaticMesh();
		if (FAILED(staticmesh->LoadMesh(_filepath, _filename))) // 로딩해보고 안되면
		{
			std::wstring error = _filepath + _filename;
			MessageBox(DXUTGetHWND(), error.c_str(), L"스태틱 메시 로드 FAIL", MB_ICONERROR | MB_OK); // 컷
			return E_FAIL;
		}
		resourcegroup.emplace(_tag, staticmesh);
	}
	else if(_meshtype == MeshType::DYNAMIC) // 다이나믹 메쉬
	{
		DynamicMesh* dynamicmesh = new DynamicMesh();
		if (FAILED(dynamicmesh->LoadMesh(_filepath, _filename))) // 해보고 안되면
		{
			std::wstring error = _filepath + _filename;
			MessageBox(DXUTGetHWND(), error.c_str(), L"다이나믹 메시 로드 FAIL", MB_ICONERROR | MB_OK); // 컷
			return E_FAIL;
		}
		resourcegroup.emplace(_tag, dynamicmesh);
	}
	return S_OK;
}

HRESULT ResourceManager::AddShader(std::wstring _filepath, std::wstring _tag)
{
	auto& iter = resourcegroup.find(_tag); // 쉐이더 찾아
	if (iter != resourcegroup.end()) // 있으면
	{
		std::wstring error = _tag;
		MessageBox(DXUTGetHWND(), error.c_str(), L"중복 셰이더", MB_ICONERROR | MB_OK); // 안대요
		return E_FAIL;
	}
	Shader* shader = new Shader(); // 있으면 추가
	if(FAILED(shader->CreateShader(_filepath))) // 함 만들어보고 안되면
		return E_FAIL; // 컷
	resourcegroup.emplace(_tag, shader); // 되면 추가
	return S_OK;
}

Resources* ResourceManager::LoadResource(std::wstring _tag) // 얘는 어따써
{
	auto& iter = resourcegroup.find(_tag); // 찾아
	if (iter != resourcegroup.end()) // 있으면
		return resourcegroup[_tag]->Clone(); // 복사한걸 리턴
	return nullptr;
}

void ResourceManager::ReleaseResources(void) // 해제
{
	for (auto& iter : resourcegroup)
	{
		iter.second->Release();
	}
	resourcegroup.clear();
}
