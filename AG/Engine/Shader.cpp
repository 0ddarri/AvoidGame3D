#include "DXUT.h"
#include "Shader.h"
USING(Engine)

Shader::Shader(void)
{
}

Shader::Shader(const Shader& cp) :
	Resources(cp),
	effect(cp.effect),
	errormsg(cp.errormsg)
{
	effect->AddRef(); // ??
	if(errormsg != nullptr)
		errormsg->AddRef();
}

Shader::~Shader(void)
{
}

void Shader::ResetDevice(void)
{
	effect->OnResetDevice();
}

void Shader::LostDevice(void)
{
	effect->OnLostDevice();
}

HRESULT Shader::CreateShader(std::wstring _filepath)
{
	HRESULT hr = S_OK;
	hr = D3DXCreateEffectFromFile(DEVICE, _filepath.c_str(), NULL, NULL, D3DXSHADER_DEBUG, NULL, &effect, &errormsg); // 파일로부터 이펙트 불러오기래
	if (FAILED(hr))
	{
		MessageBox(DXUTGetHWND(), static_cast<WCHAR*>(errormsg->GetBufferPointer()), L"Shader Create Error", MB_OK); // 셰이더 에러창을 띄우는듯?
		hr = E_FAIL;
		return hr;
	}
	else if(errormsg != nullptr)
		MessageBox(DXUTGetHWND(), static_cast<WCHAR*>(errormsg->GetBufferPointer()), L"Shader Warning", MB_OK); // 이것도 셰이더 에러창

	return hr;
}

HRESULT Shader::SetupTable(const D3DXMATRIX& _world) // 얘는 뭐하는앤지 모르겠네
{
	D3DXMATRIX matView, matProj;
	DEVICE->GetTransform(D3DTS_VIEW, &matView);
	DEVICE->GetTransform(D3DTS_PROJECTION, &matProj);

	// 이거 셰이더에 연동하는 뭐시기 그런건데
	effect->SetValue((D3DXHANDLE)"g_matWorld", &_world, sizeof(D3DXMATRIX));
	effect->SetValue((D3DXHANDLE)"g_matView", &matView, sizeof(D3DXMATRIX));
	effect->SetValue((D3DXHANDLE)"g_matProj", &matProj, sizeof(D3DXMATRIX));
	return S_OK;
}

void Shader::Free(void)
{
	Safe_Release(effect);
	if(errormsg != nullptr)
		errormsg->Release();
}

Resources* Shader::Clone(void)
{
	return new Shader(*this);
}
