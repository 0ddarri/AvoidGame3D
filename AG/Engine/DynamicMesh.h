#pragma once
#include "Resources.h"
NAMESPACE(Engine)
class AnimationController;
class MeshHierarchy;

// ���� �޽� Ŭ����
class DynamicMesh final : public Resources
{
private:
    D3DXFRAME* rootFrame; // �̰Ÿ� ���ؼ� �� ���������� �����
    MeshHierarchy* hierarchy; // ����?
    AnimationController* anicontroller; // �ִϸ��̼� ��Ʈ�ѷ�
    std::list<D3DXMESHCONTAINER_DERIVED*> meshcontainergroup; // ��ȯ ������ ���� ���� �޽� ��ü�� ĸ��ȭ?
    D3DXMATRIX* parent; // �θ�����ε�

    D3DXVECTOR3* vertexposition; // ���ؽ���ǥ�ε�
    ULONG vertexcount; // ���ؽ������ε�
protected:
public:
    explicit DynamicMesh(void);
    explicit DynamicMesh(D3DXMATRIX* _parent);
    explicit DynamicMesh(const DynamicMesh& rhs);
    virtual ~DynamicMesh(void);

    HRESULT LoadMesh(std::wstring _filepath, std::wstring _filename); // �޽� �ε�
   
    void RenderMesh(LPD3DXEFFECT& _effect); // �޽� ������
    void RenderMesh(void);

    void RenderNoSkinnedMesh(LPD3DXEFFECT& _effect); // ��Ų?
    void DrawFrame(LPD3DXFRAME _frame);
    void DrawMeshContainer(LPD3DXMESHCONTAINER _meshcontainer, LPD3DXFRAME _frame);

    void RenderNoSkinnedMesh(void);
    void DrawFrameWithEffect(LPD3DXFRAME _frame, LPD3DXEFFECT& _effect);
    void DrawMeshContainerWithEffect(LPD3DXMESHCONTAINER _meshcontainer, LPD3DXFRAME _frame, LPD3DXEFFECT& _effect);

    D3DXFRAME_DERIVED* FindBone(const std::wstring& _bonename);

    void SetParent(D3DXMATRIX* _parent);
    void SetAnimationSet(UINT& _index);
    void ResetAnimation(void);
    void PlayAnimation(const FLOAT& _dt, const FLOAT& _movelimit = 0.1f);
    void UpdateFrameMatrices(D3DXFRAME_DERIVED* _frame, const D3DXMATRIX* _parentMatrix, const float& _movelimit = 0.1f);
    void SetUpFrameMatrixPointer(D3DXFRAME_DERIVED* _frame);
    void ComputeVertexPosition(void);

    LPD3DXVECTOR3 GetVertexposition(void) { return vertexposition; }
    ULONG GetVertexcount(void) { return vertexcount; }
    bool GetIsAnimationEnd(void) const;

    Resources* Clone(void) override;
    void Free(void) override;
};
END
