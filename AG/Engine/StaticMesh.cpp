#include "DXUT.h"
#include "StaticMesh.h"

USING(Engine)

StaticMesh::StaticMesh(void)
{

}

StaticMesh::StaticMesh(std::wstring _filepath, std::wstring _filename)
{
	LoadMesh(_filepath, _filename); // 메시를 로드한다
}

StaticMesh::StaticMesh(const StaticMesh& cp) :
	Resources(cp),
	adjacencyBuffer(cp.adjacencyBuffer),
	subsetBuffer(cp.subsetBuffer),
	mesh(cp.mesh),
	subsetCount(cp.subsetCount),
	oriMesh(cp.oriMesh),
	numVertex(cp.numVertex),
	stride(cp.stride),
	vertexPosition(cp.vertexPosition),
	index(cp.index),
	numFaces(cp.numFaces)
{
	textures = new LPDIRECT3DTEXTURE9[cp.subsetCount];
	for (UINT i = 0; i < cp.subsetCount; ++i)
	{
		textures[i] = nullptr;
		if (cp.textures[i] == nullptr)
			continue;
		textures[i] = cp.textures[i];
		textures[i]->AddRef();
	}
	mesh->AddRef();
	oriMesh->AddRef();
	adjacencyBuffer->AddRef();
	subsetBuffer->AddRef();
}

StaticMesh::~StaticMesh(void)
{

}

HRESULT StaticMesh::LoadMesh(std::wstring _filepath, std::wstring _filename)
{

	HRESULT hr;
	WCHAR finalpath[MAX_PATH];
	
	::lstrcpy(finalpath, _filepath.c_str()); // 복사
	::lstrcat(finalpath, _filename.c_str()); // 뒤에 결합

	// X파일로 로드하네
	hr = D3DXLoadMeshFromX(finalpath, 
						   D3DXMESH_MANAGED, 
						   DEVICE, 
						   &adjacencyBuffer, 
						   &subsetBuffer, 
						   NULL, 
						   &subsetCount, 
						   &oriMesh);

	ULONG FVF = oriMesh->GetFVF(); // orimesh의 FVF를 받아온대
	
	if (!(FVF & D3DFVF_NORMAL)) // FVF에 노말이 없으면
	{
		oriMesh->CloneMeshFVF(oriMesh->GetOptions(), FVF | D3DFVF_NORMAL, DEVICE, &mesh); // 메쉬를 복사하는건가
		D3DXComputeNormals(mesh, static_cast<ULONG*>(adjacencyBuffer->GetBufferPointer())); // 각 정점의 노말을 계산한대
	}
	else
	{
		oriMesh->CloneMeshFVF(oriMesh->GetOptions(), FVF, DEVICE, &mesh); 
	}

	void* pVertex = nullptr;

	numVertex = mesh->GetNumVertices();
	numFaces = mesh->GetNumFaces();

	vertexPosition = new D3DXVECTOR3[numVertex]; // 동적배열 할당

	index = new INDEX16[numFaces]; // 동적배열 할당 
	INDEX16* pIndex = nullptr;

	mesh->LockIndexBuffer(0, reinterpret_cast<void**>(&pIndex)); // lock하네 인덱스버퍼 작업하려는건가
	memcpy(index, pIndex, sizeof(INDEX16) * numFaces); // pindex에 복사하는거같네
	mesh->UnlockIndexBuffer(); // unlock

	mesh->LockVertexBuffer(0, &pVertex); // 버텍스작업하려는듯

	D3DVERTEXELEMENT9 element[MAX_FVF_DECL_SIZE]; // MAX_FVF_DECL_SIZE 만큼 배열할당
	::ZeroMemory(element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE); // element 초기화
	// MAX_FVF_DECL_SIZE 얘보면 +1 되어있는데 그거는 끝을 의미하는듯

	mesh->GetDeclaration(element); // 정점들에 대한 정보를 싹 꺼내서 element에 담는다

	UCHAR offSet = 0;
	for (ULONG i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		//Usage : 정점데이터의 사용법이래
		if (element[i].Usage == D3DDECLUSAGE_POSITION) // 변하지 않는 포지션이면? 스태틱이여서 저렇게하는건가 
		{
			offSet = static_cast<UCHAR>(element[i].Offset); // 오프셋을 담네
			break;
		}
	}
	stride = D3DXGetFVFVertexSize(FVF); // FVF에 저장된 정점의 사이즈?
	for (ULONG i = 0; i < numVertex; ++i)
		vertexPosition[i] = *(reinterpret_cast<D3DXVECTOR3*>((static_cast<UCHAR*>(pVertex) + (i * stride + offSet)))); // 뭐야이건또
		// vertexPosition은 배열이 아닌데 어떻게 배열처럼썼지?
	subset = static_cast<D3DXMATERIAL*>(subsetBuffer->GetBufferPointer()); // 버퍼 내 데이터들 포인터 받아온대
	
	textures = new LPDIRECT3DTEXTURE9[subsetCount]; // 머여
	for (ULONG i = 0; i < subsetCount; ++i)
	{
		textures[i] = nullptr;
		if (subset[i].pTextureFilename == nullptr)
			continue;
		WCHAR filename[MAX_PATH] = L"";
		::lstrcpy(finalpath, _filepath.c_str()); // 파일 경로 복사
		MultiByteToWideChar(CP_ACP, 0, subset[i].pTextureFilename, strlen(subset[i].pTextureFilename), filename, MAX_PATH); // ANSI 를 유니코드로 바꾼대
		::lstrcat(finalpath, filename); // 붙여
		hr = D3DXCreateTextureFromFile(DEVICE,
									   finalpath, 
									   &textures[i]); // texture 파일에서 찾아서 만드는데 도대체 경로를 어디서 받아오는거야
	}
	mesh->UnlockVertexBuffer(); // unlock
	return hr; // 텍스처 만들어졌으면 S_OK였나 리턴하겠지?
}

void StaticMesh::RenderMesh(void) // 메쉬 렌더링
{
	for (int i = 0; i < subsetCount; ++i)
	{
		DEVICE->SetTexture(0, textures[i]);
		mesh->DrawSubset(i);
	}
}

void StaticMesh::RenderMesh(LPD3DXEFFECT& _effect) // 얘는 뭔 렌더링이야 이펙트 렌더링?
{
	for (int i = 0; i < subsetCount; ++i)
	{
		// 이펙트?
		_effect->SetTexture((D3DXHANDLE)"g_BaseTexture", textures[i]);
		_effect->CommitChanges();
		mesh->DrawSubset(i);
	}
}

Resources* StaticMesh::Clone(void) // 복사 맞는듯
{
	return new StaticMesh(*this);
}

void StaticMesh::Free(void) // 해제
{
	for (ULONG i = 0; i < subsetCount; ++i)
		Safe_Release(textures[i]);
	Safe_Release(mesh);
	Safe_Release(oriMesh);
	Safe_Release(adjacencyBuffer);
	Safe_Release(subsetBuffer);
	if (!isCloned)
	{
		SAFE_DELETE_ARRAY(index);
		SAFE_DELETE_ARRAY(vertexPosition);
	}
	SAFE_DELETE_ARRAY(textures);
}
