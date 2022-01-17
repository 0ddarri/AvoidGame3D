#pragma once
#include "Resources.h"

NAMESPACE(Engine)
class StaticMesh final : public Resources
{
private:
    LPD3DXMESH          oriMesh         = nullptr; // 원래메쉬?
    LPD3DXMESH          mesh            = nullptr; // 걍매쉬?
    LPD3DXBUFFER        adjacencyBuffer = nullptr; // 근접정보 버퍼?
    LPD3DXBUFFER        subsetBuffer    = nullptr; // 서브셋버퍼?
    LPD3DXMATERIAL      subset          = nullptr; // 서브셋?
    LPDIRECT3DTEXTURE9* textures        = nullptr; // 텍스처?
    D3DXVECTOR3*        vertexPosition  = nullptr; // 버텍스 위치? 근데 버텍스는 ㅈㄴ 많은데 뭐지 하나하나 찍는건가
    INDEX16*            index           = nullptr; // 인덱스 어따쓰는거야
    ULONG               subsetCount     = 0; // 서브셋 개수인듯
    ULONG               numVertex       = 0; // 버텍스 개수인듯
    ULONG               stride          = 0; // ?
    ULONG               numFaces        = 0; // 면 개수인듯
    
protected:
public:
    explicit StaticMesh(void);
    explicit StaticMesh(std::wstring _filepath, std::wstring _filename);
    explicit StaticMesh(const StaticMesh& cp);
    virtual ~StaticMesh(void);

    LPD3DXMESH GetMesh(void) { return mesh; }


    HRESULT LoadMesh(std::wstring _filepath, std::wstring _filename);

    void Render(const FLOAT& dt) override {};

    void RenderMesh(void);
    void RenderMesh(LPD3DXEFFECT& _effect);

    Resources* Clone(void) override;
    void Free(void) override;
};
END