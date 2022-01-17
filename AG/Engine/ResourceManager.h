#pragma once
#include "Singleton.h"

NAMESPACE(Engine)
class Resources;
class ResourceManager final : public Singleton<ResourceManager>
{
private:
    std::map<std::wstring, Resources*> resourcegroup; // 리소스그룹이래
protected:

public:
    HRESULT AddTexture(std::wstring _filepath, std::wstring _tag, const UINT& _texturecount = 1); // 누가봐도 텍스처추가
    HRESULT AddMesh(MeshType _meshtype, std::wstring _filepath, std::wstring _filename, std::wstring _tag); // 누가봐도 메시추가
    HRESULT AddShader(std::wstring _filepath, std::wstring _tag); // 누가봐도 쉐이더 추가

    Resources* LoadResource(std::wstring _tag); // 누가봐도 ?

    void ReleaseResources(void);
};
END
