#include "DXUT.h"
#include "MeshHierarchy.h"
#include "AnimationController.h"
#include "DynamicMesh.h"
USING(Engine)

// 초기화
DynamicMesh::DynamicMesh(void) :
	rootFrame(nullptr),
	hierarchy(nullptr),
	anicontroller(nullptr)
{
	parent = nullptr;
}

// 초기화 + 부모세팅
DynamicMesh::DynamicMesh(D3DXMATRIX* _parent) :
	rootFrame(nullptr),
	hierarchy(nullptr),
	anicontroller(nullptr)
{
	parent = _parent;
}

// 초기화 + 애니메이션 컨트롤러 복사?
DynamicMesh::DynamicMesh(const DynamicMesh& rhs) :
	Resources(rhs),
	rootFrame(rhs.rootFrame),
	hierarchy(rhs.hierarchy),
	meshcontainergroup(rhs.meshcontainergroup),
	parent(rhs.parent),
	vertexposition(rhs.vertexposition)
{
	anicontroller = rhs.anicontroller->Clone();
}

DynamicMesh::~DynamicMesh(void)
{
}

// 메쉬 로드
HRESULT DynamicMesh::LoadMesh(std::wstring _filepath, std::wstring _filename)
{
	// 경로 만들기
	TCHAR fullpath[MAX_PATH] = L"";
	lstrcpy(fullpath, _filepath.c_str());
	lstrcat(fullpath, _filename.c_str());

	// 계층?
	hierarchy = new MeshHierarchy(_filepath);
	if (hierarchy == nullptr)
		return E_FAIL;

	// 애니메이션컨트롤러?
	LPD3DXANIMATIONCONTROLLER tempcontroller = nullptr;
	if (FAILED(D3DXLoadMeshHierarchyFromX(fullpath, D3DXMESH_MANAGED, DEVICE, hierarchy, NULL, &rootFrame, &tempcontroller)))
		return E_FAIL;
	// D3DXLoadMeshHierarchyFromX : 애니메이션처리할 때 이거 써야된다는데 .x파일에서 첫번째 계층을 로드한대
	
	// 애니메이션쪽인거같네
	anicontroller = new AnimationController(tempcontroller); // tempcontroller를 받아오는?
	if (anicontroller == nullptr) // null값이면
		return E_FAIL;

	Safe_Release(tempcontroller); // 이미 복사했으니 필요없으니까 지워

	D3DXMATRIX tempMatrix;
	UpdateFrameMatrices(static_cast<D3DXFRAME_DERIVED*>(rootFrame), D3DXMatrixIdentity(&tempMatrix));
	SetUpFrameMatrixPointer(static_cast<D3DXFRAME_DERIVED*>(rootFrame));
	ComputeVertexPosition();
	return S_OK;
}

// 메시 렌더링파트인듯
void DynamicMesh::RenderMesh(LPD3DXEFFECT& _effect)
{
	for (auto& meshcontainer : meshcontainergroup)
	{
		D3DXMESHCONTAINER_DERIVED* tempMeshcontainer = meshcontainer; // 메시컨테이너
		for (ULONG i = 0; i < tempMeshcontainer->numBones; ++i) // 메시컨테이너에 있는 뼈 개수만큼
		{
			// 암튼 행렬 설정같은데
			tempMeshcontainer->renderingMatrix[i] = tempMeshcontainer->frameOffsetMatrix[i] * (*tempMeshcontainer->frameCombinedMatrix[i]);
		}

		void* srcvtx = nullptr; // ?
		void* destvtx = nullptr; // ?

		tempMeshcontainer->originalMesh->LockVertexBuffer(0, &srcvtx);
		tempMeshcontainer->MeshData.pMesh->LockVertexBuffer(0, &destvtx);

		// 음 메쉬를 갱신?
		tempMeshcontainer->pSkinInfo->UpdateSkinnedMesh(tempMeshcontainer->renderingMatrix, NULL, srcvtx, destvtx);
		
		for (UINT i = 0; i < tempMeshcontainer->NumMaterials; ++i)
		{
			_effect->SetTexture((D3DXHANDLE)"g_BaseTexture", tempMeshcontainer->textures[i]); // 뭔가 쉐이더랑 머시기 하는거 같은데
			_effect->CommitChanges(); // 공유 매개변수 업데이트?
			tempMeshcontainer->MeshData.pMesh->DrawSubset(i);
		}
		tempMeshcontainer->originalMesh->UnlockVertexBuffer();
		tempMeshcontainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void DynamicMesh::RenderMesh(void) // 쉐이더 없이 하는 렌더링인가
{
	for (auto& meshcontainer : meshcontainergroup)
	{
		D3DXMESHCONTAINER_DERIVED* tempMeshcontainer = meshcontainer;
		for (ULONG i = 0; i < tempMeshcontainer->numBones; ++i)
		{
			tempMeshcontainer->renderingMatrix[i] = tempMeshcontainer->frameOffsetMatrix[i] * (*tempMeshcontainer->frameCombinedMatrix[i]);
		}

		void* srcvtx = nullptr;
		void* destvtx = nullptr;

		tempMeshcontainer->originalMesh->LockVertexBuffer(0, &srcvtx);
		tempMeshcontainer->MeshData.pMesh->LockVertexBuffer(0, &destvtx);

		tempMeshcontainer->pSkinInfo->UpdateSkinnedMesh(tempMeshcontainer->renderingMatrix, NULL, srcvtx, destvtx);

		for (UINT i = 0; i < tempMeshcontainer->NumMaterials; ++i)
		{
			DEVICE->SetTexture(0, tempMeshcontainer->textures[i]);
			tempMeshcontainer->MeshData.pMesh->DrawSubset(i);
		}

		tempMeshcontainer->originalMesh->UnlockVertexBuffer();
		tempMeshcontainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

// 아 걍 프레임으로 그리는건가
void DynamicMesh::DrawFrameWithEffect(LPD3DXFRAME _frame, LPD3DXEFFECT& _effect)
{
	LPD3DXMESHCONTAINER meshcontainer = _frame->pMeshContainer;

	while (meshcontainer != nullptr) 
	{
		DrawMeshContainerWithEffect(meshcontainer, _frame, _effect); 
		meshcontainer = meshcontainer->pNextMeshContainer;
	}

	if (_frame->pFrameSibling != nullptr)
	{
		DrawFrameWithEffect(_frame->pFrameSibling, _effect);
	}

	if (_frame->pFrameFirstChild != nullptr)
	{
		DrawFrameWithEffect(_frame->pFrameFirstChild, _effect);
	}
}

// 메쉬컨테이너 그리기?
void DynamicMesh::DrawMeshContainerWithEffect(LPD3DXMESHCONTAINER _meshcontainer, LPD3DXFRAME _frame, LPD3DXEFFECT& _effect)
{
	D3DXMESHCONTAINER_DERIVED* tempmeshcontainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(_meshcontainer);
	D3DXFRAME_DERIVED* tempframe = static_cast<D3DXFRAME_DERIVED*>(_frame);

	D3DXMATRIX result = tempframe->combinedTransformMatrix;
	if (parent)
		result *= *parent;
	_effect->SetValue((D3DXHANDLE)"g_matWorld", &result, sizeof(D3DXMATRIX));

	for (int i = 0; i < tempmeshcontainer->NumMaterials; ++i)
	{
		_effect->SetTexture((D3DXHANDLE)"g_BaseTexture", tempmeshcontainer->textures[i]);
		_effect->CommitChanges();
		tempmeshcontainer->MeshData.pMesh->DrawSubset(i);
	}
}

void DynamicMesh::RenderNoSkinnedMesh(LPD3DXEFFECT& _effect)
{
	DrawFrameWithEffect(rootFrame, _effect);
}

void DynamicMesh::RenderNoSkinnedMesh(void)
{
	DrawFrame(rootFrame);
}

void DynamicMesh::DrawFrame(LPD3DXFRAME _frame)
{
	LPD3DXMESHCONTAINER meshcontainer = _frame->pMeshContainer;

	while (meshcontainer != nullptr)
	{
		DrawMeshContainer(meshcontainer, _frame);
		meshcontainer = meshcontainer->pNextMeshContainer;
	}

	if (_frame->pFrameSibling != nullptr)
	{
		DrawFrame(_frame->pFrameSibling);
	}

	if (_frame->pFrameFirstChild != nullptr)
	{
		DrawFrame(_frame->pFrameFirstChild);
	}
}

void DynamicMesh::DrawMeshContainer(LPD3DXMESHCONTAINER _meshcontainer, LPD3DXFRAME _frame)
{
	D3DXMESHCONTAINER_DERIVED* tempmeshcontainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(_meshcontainer);
	D3DXFRAME_DERIVED* tempframe = static_cast<D3DXFRAME_DERIVED*>(_frame);

	D3DXMATRIX result = tempframe->combinedTransformMatrix;
	result *= *parent;
	DEVICE->SetTransform(D3DTS_WORLD, &result);
	for (int i = 0; i < tempmeshcontainer->NumMaterials; ++i)
	{
		DEVICE->SetMaterial(&tempmeshcontainer->pMaterials[i].MatD3D);
		DEVICE->SetTexture(0, tempmeshcontainer->textures[i]);
		tempmeshcontainer->MeshData.pMesh->DrawSubset(i);
	}
}

D3DXFRAME_DERIVED* DynamicMesh::FindBone(const std::wstring& _bonename)
{
	if (rootFrame == nullptr)
		return nullptr;
	char bonename[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, _bonename.c_str(), _bonename.length(), bonename, MAX_PATH, NULL, NULL);

	return static_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(rootFrame, bonename));
}

void DynamicMesh::SetParent(D3DXMATRIX* _parent)
{
	parent = _parent;
}

void DynamicMesh::SetAnimationSet(UINT& _index)
{
	anicontroller->SetAnimationSet(_index);
}

void DynamicMesh::ResetAnimation(void)
{
	anicontroller->ResetAnimation();
}

void DynamicMesh::PlayAnimation(const FLOAT& _dt, const FLOAT& _movelimit)
{
	anicontroller->PlayAnimation(_dt);
	D3DXMATRIX tempMatrix;
	UpdateFrameMatrices(static_cast<D3DXFRAME_DERIVED*>(rootFrame), D3DXMatrixIdentity(&tempMatrix));
}

// 모든 뼈의 월드변환행렬을 계산?
void DynamicMesh::UpdateFrameMatrices(D3DXFRAME_DERIVED* _frame, const D3DXMATRIX* _parentMatrix, const float& _movelimit)
{
	if (_frame == nullptr) // 뼈 없으면 컷
		return;

	if (_parentMatrix == nullptr) // 부모행렬 없으면 
		_frame->combinedTransformMatrix = _frame->TransformationMatrix; // 월드행렬은 로컬행렬이다
	else // 있으면
		_frame->combinedTransformMatrix = _frame->TransformationMatrix * (*_parentMatrix); // 부모행렬곱

	if (_frame->pFrameSibling != nullptr) // 형제 뼈 포인터가 있으면
		UpdateFrameMatrices(static_cast<D3DXFRAME_DERIVED*>(_frame->pFrameSibling), _parentMatrix, _movelimit); // 걔도 계산

	if (_frame->pFrameFirstChild != nullptr) // 자식 뼈 있으면
		UpdateFrameMatrices(static_cast<D3DXFRAME_DERIVED*>(_frame->pFrameFirstChild), &_frame->combinedTransformMatrix, _movelimit); // 걔도 계산
}

// 뼈의 행렬 포인터 셋업?
void DynamicMesh::SetUpFrameMatrixPointer(D3DXFRAME_DERIVED* _frame)
{
	// 메쉬컨테이너 있으면
	if (_frame->pMeshContainer != nullptr)
	{
		D3DXMESHCONTAINER_DERIVED* tempMeshcontainer = static_cast<D3DXMESHCONTAINER_DERIVED*>(_frame->pMeshContainer);
		for (ULONG i = 0; i < tempMeshcontainer->numBones; ++i) // 뼈 개수만큼 for
		{
			const char* name = tempMeshcontainer->pSkinInfo->GetBoneName(i); // 뼈 이름을 가져옴
			D3DXFRAME_DERIVED* frame = static_cast<D3DXFRAME_DERIVED*>(D3DXFrameFind(rootFrame, name)); // 이름으로 뼈 찾아
			tempMeshcontainer->frameCombinedMatrix[i] = &frame->combinedTransformMatrix; // 뼈의 월드행렬을 줘
		}
		meshcontainergroup.push_back(tempMeshcontainer); // 메쉬컨테이너그룹에 넣어
	}

	if (_frame->pFrameSibling != nullptr) // 형제있으면
		SetUpFrameMatrixPointer(static_cast<D3DXFRAME_DERIVED*>(_frame->pFrameSibling)); // 이걸 해줘

	if (_frame->pFrameFirstChild != nullptr) // 자식 있으면
		SetUpFrameMatrixPointer(static_cast<D3DXFRAME_DERIVED*>(_frame->pFrameFirstChild)); // 이걸 해줘
}
 // 버텍스 포지션 계산
void DynamicMesh::ComputeVertexPosition(void)
{
	void* vertex = nullptr; // 범용 포인터 : 형식에 관계없이 포인터를 전달하고 싶을 때
	// 정점은 정보손실이 없어야 하기 때문에 범용포인터 써버려

	vertexcount = 0;

	// 모든 메쉬들의 버텍스 개수를 받는다
	for (auto& pMeshContainer : meshcontainergroup)
		vertexcount += pMeshContainer->originalMesh->GetNumVertices();

	vertexposition = new D3DXVECTOR3[vertexcount]; // 버텍스 개수만큼 버텍스 포지션 동적배열 할당

	LPD3DXMESH mesh = nullptr; // 메쉬?

	ULONG currentIndex = 0; // 현재 인덱스?

	for (auto& pMeshContainer : meshcontainergroup)
	{
		mesh = pMeshContainer->originalMesh; // 암튼 오리지널 메쉬를 가져와

		mesh->LockVertexBuffer(0, (void**)&vertex); // 버텍스잠금

		// 정점의 특성을 담는 변수
		// MAX_FVF_DECL_SIZE : 한 정점이 가지고 있을 수 있는 최대 FVF 개수
		D3DVERTEXELEMENT9 declare[MAX_FVF_DECL_SIZE];
		::ZeroMemory(declare, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

		// 정점의 특성을 가져온다.
		mesh->GetDeclaration(declare);
		ULONG FVF = mesh->GetFVF();
		ULONG ULvertexcount = mesh->GetNumVertices();

		UCHAR offset = 0;

		// 위치에 해당하는 시맨틱 정보가 존재하는지 찾는다.
		for (ULONG i = 0; i < MAX_FVF_DECL_SIZE; ++i)
		{
			// 시맨틱정보가 위치 정보일 경우
			if (D3DDECLUSAGE_POSITION == declare[i].Usage)
			{
				offset = (UCHAR)declare[i].Offset;
				break;
			}
		}

		// 현재 메쉬의 정점 크기를 저장한다.
		ULONG stride = D3DXGetFVFVertexSize(FVF);

		// 포인터 연산을 통해 해당 메쉬의 정점 좌표를 저장한다.
		for (ULONG i = 0; i < ULvertexcount; ++i)
			vertexposition[currentIndex++] = *(reinterpret_cast<D3DXVECTOR3*>((static_cast<UCHAR*>(vertex) + (i * stride + offset))));
		mesh->UnlockVertexBuffer();
	}
}

bool DynamicMesh::GetIsAnimationEnd(void) const
{
	return anicontroller->IsAnimationEnd();
}

Resources* DynamicMesh::Clone(void)
{
	return new DynamicMesh(*this);
}

void DynamicMesh::Free(void)
{
	parent = nullptr;
	Safe_Release(anicontroller);
	if (!isCloned)
	{
		Safe_Release(hierarchy);
		hierarchy->DestroyFrame(rootFrame);
		Safe_Delete_Array(vertexposition);
	}
	meshcontainergroup.clear();
}
