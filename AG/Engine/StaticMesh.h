#pragma once
#include "Resources.h"

NAMESPACE(Engine)
class StaticMesh final : public Resources
{
private:
    LPD3DXMESH          oriMesh         = nullptr; // �����޽�?
    LPD3DXMESH          mesh            = nullptr; // ���Ž�?
    LPD3DXBUFFER        adjacencyBuffer = nullptr; // �������� ����?
    LPD3DXBUFFER        subsetBuffer    = nullptr; // ����¹���?
    LPD3DXMATERIAL      subset          = nullptr; // �����?
    LPDIRECT3DTEXTURE9* textures        = nullptr; // �ؽ�ó?
    D3DXVECTOR3*        vertexPosition  = nullptr; // ���ؽ� ��ġ? �ٵ� ���ؽ��� ���� ������ ���� �ϳ��ϳ� ��°ǰ�
    INDEX16*            index           = nullptr; // �ε��� ������°ž�
    ULONG               subsetCount     = 0; // ����� �����ε�
    ULONG               numVertex       = 0; // ���ؽ� �����ε�
    ULONG               stride          = 0; // ?
    ULONG               numFaces        = 0; // �� �����ε�
    
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