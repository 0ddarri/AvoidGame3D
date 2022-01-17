#include "DXUT.h"
#include "MeshHierarchy.h"
USING(Engine)
void MeshHierarchy::AllocateName(char** destination, const char* name) // �̸� �Ҵ�?
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

    AllocateName(&frame->Name, Name); // �� �̸� ����

    frame->combinedTransformMatrix = *D3DXMatrixIdentity(&frame->TransformationMatrix); // ��� �Ѵ� �ʱ�ȭ?
    
    *ppNewFrame = frame; // ������ �� ����
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) MeshHierarchy::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
    D3DXMESHCONTAINER_DERIVED* meshcontainer = new D3DXMESHCONTAINER_DERIVED; // �޽��鿡 ���� ����?
    ZeroMemory(meshcontainer, sizeof(D3DXMESHCONTAINER_DERIVED));

    AllocateName(&meshcontainer->Name, Name); // �޽������̳� �̸� ����
    meshcontainer->MeshData.Type = D3DXMESHTYPE_MESH; // Ÿ�� ����

    LPD3DXMESH mesh = pMeshData->pMesh;

    ULONG faceNumbers = mesh->GetNumFaces();
    meshcontainer->pAdjacency = new ULONG[faceNumbers * 3]; // �� ����
    memcpy(meshcontainer->pAdjacency, pAdjacency, sizeof(ULONG) * faceNumbers * 3); // ���礿��

    ULONG FVF = mesh->GetFVF(); // FVF �޾ƿ�

    if (!(FVF & D3DFVF_NORMAL)) // FVF�� �븻 ������
    {
        mesh->CloneMeshFVF(mesh->GetOptions(), FVF | D3DFVF_NORMAL, DEVICE, &meshcontainer->MeshData.pMesh); // �븻�� �־�������� ����?
        D3DXComputeNormals(meshcontainer->MeshData.pMesh, meshcontainer->pAdjacency); // �븻 �α׸� ���
    }
    else // �븻 ������
        mesh->CloneMeshFVF(mesh->GetOptions(), FVF, DEVICE, &meshcontainer->MeshData.pMesh); // �� �Ѱ�

    meshcontainer->NumMaterials = (NumMaterials = 0 ? 1 : NumMaterials); // ���͸��� ��ȣ? ����?

    meshcontainer->pMaterials = new D3DXMATERIAL[meshcontainer->NumMaterials]; // ���͸��� ������ŭ �����迭 �Ҵ�
    ZeroMemory(meshcontainer->pMaterials, sizeof(D3DXMATERIAL) * meshcontainer->NumMaterials); // �ʱ�ȭ

    meshcontainer->textures = new LPDIRECT3DTEXTURE9[meshcontainer->NumMaterials]; // �ؽ�ó�� ���͸����̶� �������� 
    ZeroMemory(meshcontainer->textures, sizeof(LPDIRECT3DTEXTURE9) * meshcontainer->NumMaterials); // �ʱ�ȭ

    if (NumMaterials != 0) // ���͸��� ���� 0�� �ƴϸ�
    {
        memcpy(meshcontainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * meshcontainer->NumMaterials); // ����
        for (ULONG i = 0; i < meshcontainer->NumMaterials; ++i) // ������ŭ for��
        {
            TCHAR fullpath[MAX_PATH] = L"";
            TCHAR filename[256] = L"";


            MultiByteToWideChar(CP_ACP, 0, meshcontainer->pMaterials[i].pTextureFilename,
                strlen(meshcontainer->pMaterials[i].pTextureFilename),
                filename,
                256); // �� ���� ���� ��ȯ�ϴ°ž� ANSI���� �����ڵ忴��

            ::lstrcpy(fullpath, filepath.c_str()); // ����
            ::lstrcat(fullpath, filename); // �ٿ�

            if (FAILED(D3DXCreateTextureFromFile(DEVICE,
                fullpath,
                &meshcontainer->textures[i]))) // �ؽ�ó �����µ� �����ϸ�
                return E_FAIL; // ��
        }
    }
    else // ���͸��� 0�̸�
    {
        meshcontainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0, 0, 0, 0);
        meshcontainer->pMaterials[0].MatD3D.Power = 0;

        meshcontainer->textures[0] = nullptr;
        // ���� �� �ʱ�ȭ�۾��ΰŰ���
    }

    // MeshFVF ����
    meshcontainer->MeshData.pMesh->CloneMeshFVF(meshcontainer->MeshData.pMesh->GetOptions(),
        meshcontainer->MeshData.pMesh->GetFVF(),
        DEVICE,
        &meshcontainer->originalMesh);

    // ��Ų? �װԹ��� ��ư ��Ų ������ ������
    if (pSkinInfo == nullptr)
    {
        *ppNewMeshContainer = meshcontainer;
        return S_OK;
    }
    // ��Ų���� ��
    meshcontainer->pSkinInfo = pSkinInfo;
    meshcontainer->pSkinInfo->AddRef();
    
    meshcontainer->numBones = meshcontainer->pSkinInfo->GetNumBones();
    
    // �ʱ�ȭ
    meshcontainer->frameOffsetMatrix = new D3DXMATRIX[meshcontainer->numBones];
    ZeroMemory(meshcontainer->frameOffsetMatrix, sizeof(D3DXMATRIX) * meshcontainer->numBones);

    // �ʱ�ȭ
    meshcontainer->frameCombinedMatrix = new D3DXMATRIX * [meshcontainer->numBones];
    ZeroMemory(meshcontainer->frameCombinedMatrix, sizeof(D3DXMATRIX*) * meshcontainer->numBones);

    // �ʱ�ȭ
    meshcontainer->renderingMatrix = new D3DXMATRIX[meshcontainer->numBones];
    ZeroMemory(meshcontainer->renderingMatrix, sizeof(D3DXMATRIX) * meshcontainer->numBones);

    // �� ������ŭ for
    for (ULONG i = 0; i < meshcontainer->numBones; ++i)
        meshcontainer->frameOffsetMatrix[i] = *meshcontainer->pSkinInfo->GetBoneOffsetMatrix(i); // ? ������ �Ѱ�

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
