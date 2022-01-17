#include "DXUT.h"
#include "MeshHierarchy.h"
USING(Engine)
void MeshHierarchy::AllocateName(char** destination, const char* name) // 이름 할당?
{
    if (name != nullptr)
    {
        UINT length = strlen(name);
        *destination = new char[length + 1];
        strcpy_s(*destination, length + 1, name);
    }
}

MeshHierarchy::MeshHierarchy(void)
{
}

MeshHierarchy::MeshHierarchy(std::wstring _path) :
    filepath(_path)
{
}

MeshHierarchy::~MeshHierarchy(void)
{
}

STDMETHODIMP_(HRESULT __stdcall) MeshHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
    D3DXFRAME_DERIVED* frame = new D3DXFRAME_DERIVED;
    ZeroMemory(frame, sizeof(D3DXFRAME_DERIVED));

    AllocateName(&frame->Name, Name); // 뼈 이름 지정

    frame->combinedTransformMatrix = *D3DXMatrixIdentity(&frame->TransformationMatrix); // 행렬 둘다 초기화?
    
    *ppNewFrame = frame; // 생성된 뼈 리턴
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) MeshHierarchy::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
    D3DXMESHCONTAINER_DERIVED* meshcontainer = new D3DXMESHCONTAINER_DERIVED; // 메쉬들에 대한 정보?
    ZeroMemory(meshcontainer, sizeof(D3DXMESHCONTAINER_DERIVED));

    AllocateName(&meshcontainer->Name, Name); // 메쉬컨테이너 이름 지정
    meshcontainer->MeshData.Type = D3DXMESHTYPE_MESH; // 타입 지정

    LPD3DXMESH mesh = pMeshData->pMesh;

    ULONG faceNumbers = mesh->GetNumFaces();
    meshcontainer->pAdjacency = new ULONG[faceNumbers * 3]; // 음 뭘까
    memcpy(meshcontainer->pAdjacency, pAdjacency, sizeof(ULONG) * faceNumbers * 3); // 복사ㅏㅏ

    ULONG FVF = mesh->GetFVF(); // FVF 받아옴

    if (!(FVF & D3DFVF_NORMAL)) // FVF에 노말 없으면
    {
        mesh->CloneMeshFVF(mesh->GetOptions(), FVF | D3DFVF_NORMAL, DEVICE, &meshcontainer->MeshData.pMesh); // 노말을 넣어버린다음 복사?
        D3DXComputeNormals(meshcontainer->MeshData.pMesh, meshcontainer->pAdjacency); // 노말 싸그리 계산
    }
    else // 노말 있으면
        mesh->CloneMeshFVF(mesh->GetOptions(), FVF, DEVICE, &meshcontainer->MeshData.pMesh); // 걍 넘겨

    meshcontainer->NumMaterials = (NumMaterials = 0 ? 1 : NumMaterials); // 머터리얼 번호? 개수?

    meshcontainer->pMaterials = new D3DXMATERIAL[meshcontainer->NumMaterials]; // 머터리얼 개수만큼 동적배열 할당
    ZeroMemory(meshcontainer->pMaterials, sizeof(D3DXMATERIAL) * meshcontainer->NumMaterials); // 초기화

    meshcontainer->textures = new LPDIRECT3DTEXTURE9[meshcontainer->NumMaterials]; // 텍스처도 머터리얼이랑 마찬가지 
    ZeroMemory(meshcontainer->textures, sizeof(LPDIRECT3DTEXTURE9) * meshcontainer->NumMaterials); // 초기화

    if (NumMaterials != 0) // 머터리얼 개수 0개 아니면
    {
        memcpy(meshcontainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * meshcontainer->NumMaterials); // 복사
        for (ULONG i = 0; i < meshcontainer->NumMaterials; ++i) // 개수만큼 for문
        {
            TCHAR fullpath[MAX_PATH] = L"";
            TCHAR filename[256] = L"";


            MultiByteToWideChar(CP_ACP, 0, meshcontainer->pMaterials[i].pTextureFilename,
                strlen(meshcontainer->pMaterials[i].pTextureFilename),
                filename,
                256); // 음 대충 뭔가 변환하는거야 ANSI에서 유니코드였나

            ::lstrcpy(fullpath, filepath.c_str()); // 복사
            ::lstrcat(fullpath, filename); // 붙여

            if (FAILED(D3DXCreateTextureFromFile(DEVICE,
                fullpath,
                &meshcontainer->textures[i]))) // 텍스처 만들어보는데 실패하면
                return E_FAIL; // 컷
        }
    }
    else // 머터리얼 0이면
    {
        meshcontainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Power = 0;

        meshcontainer->textures[0] = nullptr;
        // 뭔가 다 초기화작업인거같아
    }

    // MeshFVF 복사
    meshcontainer->MeshData.pMesh->CloneMeshFVF(meshcontainer->MeshData.pMesh->GetOptions(),
        meshcontainer->MeshData.pMesh->GetFVF(),
        DEVICE,
        &meshcontainer->originalMesh);

    // 스킨? 그게뭐지 암튼 스킨 정보가 없으면
    if (pSkinInfo == nullptr)
    {
        *ppNewMeshContainer = meshcontainer;
        return S_OK;
    }
    // 스킨정보 줘
    meshcontainer->pSkinInfo = pSkinInfo;
    meshcontainer->pSkinInfo->AddRef();
    
    meshcontainer->numBones = meshcontainer->pSkinInfo->GetNumBones();
    
    // 초기화
    meshcontainer->frameOffsetMatrix = new D3DXMATRIX[meshcontainer->numBones];
    ZeroMemory(meshcontainer->frameOffsetMatrix, sizeof(D3DXMATRIX) * meshcontainer->numBones);

    // 초기화
    meshcontainer->frameCombinedMatrix = new D3DXMATRIX * [meshcontainer->numBones];
    ZeroMemory(meshcontainer->frameCombinedMatrix, sizeof(D3DXMATRIX*) * meshcontainer->numBones);

    // 초기화
    meshcontainer->renderingMatrix = new D3DXMATRIX[meshcontainer->numBones];
    ZeroMemory(meshcontainer->renderingMatrix, sizeof(D3DXMATRIX) * meshcontainer->numBones);

    // 뼈 개수만큼 for
    for (ULONG i = 0; i < meshcontainer->numBones; ++i)
        meshcontainer->frameOffsetMatrix[i] = *meshcontainer->pSkinInfo->GetBoneOffsetMatrix(i); // ? 뭔가를 넘겨

    *ppNewMeshContainer = meshcontainer;

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) MeshHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
    Safe_Delete_Array(pFrameToFree->Name);
    if (pFrameToFree->pMeshContainer != nullptr)
        DestroyMeshContainer(pFrameToFree->pMeshContainer);
    if (pFrameToFree->pFrameSibling != nullptr)
        DestroyFrame(pFrameToFree->pFrameSibling);
    if (pFrameToFree->pFrameFirstChild != nullptr)
        DestroyFrame(pFrameToFree->pFrameFirstChild);
    Safe_Delete(pFrameToFree);
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) MeshHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
    D3DXMESHCONTAINER_DERIVED* meshcontainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(pMeshContainerToFree);
    for (ULONG i = 0; i < meshcontainer->NumMaterials; ++i)
        Safe_Release(meshcontainer->textures[i]);
    Safe_Delete_Array(meshcontainer->pAdjacency);
    Safe_Delete_Array(meshcontainer->pMaterials);
    Safe_Delete_Array(meshcontainer->Name);
    Safe_Delete_Array(meshcontainer->textures);
    Safe_Delete_Array(meshcontainer->frameOffsetMatrix);
    Safe_Delete_Array(meshcontainer->frameCombinedMatrix);
    Safe_Delete_Array(meshcontainer->renderingMatrix);
    Safe_Release(meshcontainer->MeshData.pMesh);
    Safe_Release(meshcontainer->pSkinInfo);
    Safe_Release(meshcontainer->originalMesh);
    Safe_Delete(meshcontainer);
    return S_OK;
}

DWORD MeshHierarchy::Release(void)
{
    delete this;
    return 0;
}
