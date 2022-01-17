#include "DXUT.h"
#include "Texture.h"
USING(Engine)
Texture::Texture(void)
{
}

Texture::Texture(const Texture& cp) :
	Resources(cp),
	textures(cp.textures),
	texturescount(cp.texturescount)
{
	for (int i = 0; i < texturescount; ++i)
		textures[i]->AddRef();
}

Texture::~Texture(void)
{
}

HRESULT Texture::LoadTexture(std::wstring _filepath, const UINT& _texturecount)
{
	LPDIRECT3DTEXTURE9 texture;
	texturescount = _texturecount;
	if (_texturecount <= 1) // 텍스처 카운트가 1 이하면
	{
		if (FAILED(D3DXCreateTextureFromFile(DEVICE, _filepath.c_str(), &texture))) // 파일로 텍스처 만들기
		{
			std::wcout << "FAIL : " << _filepath << std::endl;
			return E_FAIL;
		}
		std::wcout << "LOAD : " << _filepath << std::endl;
		textures.emplace_back(texture); // 넣어용
	}
	else // 1보다 크면
	{
		TCHAR finalfilepath[MAX_PATH] = L"";
		for (size_t i = 1; i <= texturescount; ++i)
		{
			wsprintf(finalfilepath, _filepath.c_str(), i); // finalfilepath 로 복사하는거같아

			if (FAILED(D3DXCreateTextureFromFile(DEVICE, finalfilepath, &texture))) // 텍스처 만들어용
				return E_FAIL;

			textures.emplace_back(texture); // 넣어용
		}
	}
	return S_OK;
}

void Texture::RenderTexture(LPD3DXEFFECT& _effect, const UINT& _index)
{
	if (textures.size() <= _index)
		return;
	_effect->SetTexture((D3DXHANDLE)L"g_BaseTexture", textures[_index]);
}

void Texture::RenderTexture(const UINT& _index)
{
	if (textures.size() <= _index)
		return;

	DEVICE->SetTexture(0, textures[_index]);
}

void Texture::Render(const FLOAT& dt)
{

}

Resources* Texture::Clone(void)
{
	return new Texture(*this);
}

void Texture::Free(void)
{
	for (int i = 0; i < texturescount; ++i)
		textures[i]->Release();
	textures.clear();
}
