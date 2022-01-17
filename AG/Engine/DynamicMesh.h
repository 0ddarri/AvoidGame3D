#pragma once
#include "Resources.h"
NAMESPACE(Engine)
class AnimationController;
class MeshHierarchy;

// 동적 메쉬 클래스
class DynamicMesh final : public Resources
{
private:
    D3DXFRAME* rootFrame; // 이거를 통해서 뼈 계층구조를 만든대
    MeshHierarchy* hierarchy; // 계층?
    AnimationController* anicontroller; // 애니메이션 컨트롤러
    std::list<D3DXMESHCONTAINER_DERIVED*> meshcontainergroup; // 변환 프레임 계층 내의 메쉬 개체를 캡슐화?
    D3DXMATRIX* parent; // 부모행렬인듯

    D3DXVECTOR3* vertexposition; // 버텍스좌표인듯
    ULONG vertexcount; // 버텍스개수인듯
protected:
public:
    explicit DynamicMesh(void);
    explicit DynamicMesh(D3DXMATRIX* _parent);
    explicit DynamicMesh(const DynamicMesh& rhs);
    virtual ~DynamicMesh(void);

    HRESULT LoadMesh(std::wstring _filepath, std::wstring _filename); // 메쉬 로드
   
    void RenderMesh(LPD3DXEFFECT& _effect); // 메쉬 렌더링
    void RenderMesh(void);

    void RenderNoSkinnedMesh(LPD3DXEFFECT& _effect); // 스킨?
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
