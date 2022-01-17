#include "DXUT.h"
#include "StaticMesh.h"

USING(Engine)

StaticMesh::StaticMesh(void)
{

}

StaticMesh::StaticMesh(std::wstring _filepath, std::wstring _filename)
{
	LoadMesh(_filepath, _filename); // �޽ø� �ε��Ѵ�
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
	
	::lstrcpy(finalpath, _filepath.c_str()); // ����
	::lstrcat(finalpath, _filename.c_str()); // �ڿ� ����

	// X���Ϸ� �ε��ϳ�
	hr = D3DXLoadMeshFromX(finalpath, 
						   D3DXMESH_MANAGED, 
						   DEVICE, 
						   &adjacencyBuffer, 
						   &subsetBuffer, 
						   NULL, 
						   &subsetCount, 
						   &oriMesh);

	ULONG FVF = oriMesh->GetFVF(); // orimesh�� FVF�� �޾ƿ´�
	
	if (!(FVF & D3DFVF_NORMAL)) // FVF�� �븻�� ������
	{
		oriMesh->CloneMeshFVF(oriMesh->GetOptions(), FVF | D3DFVF_NORMAL, DEVICE, &mesh); // �޽��� �����ϴ°ǰ�
		D3DXComputeNormals(mesh, static_cast<ULONG*>(adjacencyBuffer->GetBufferPointer())); // �� ������ �븻�� ����Ѵ�
	}
	else
	{
		oriMesh->CloneMeshFVF(oriMesh->GetOptions(), FVF, DEVICE, &mesh); 
	}

	void* pVertex = nullptr;

	numVertex = mesh->GetNumVertices();
	numFaces = mesh->GetNumFaces();

	vertexPosition = new D3DXVECTOR3[numVertex]; // �����迭 �Ҵ�

	index = new INDEX16[numFaces]; // �����迭 �Ҵ� 
	INDEX16* pIndex = nullptr;

	mesh->LockIndexBuffer(0, reinterpret_cast<void**>(&pIndex)); // lock�ϳ� �ε������� �۾��Ϸ��°ǰ�
	memcpy(index, pIndex, sizeof(INDEX16) * numFaces); // pindex�� �����ϴ°Ű���
	mesh->UnlockIndexBuffer(); // unlock

	mesh->LockVertexBuffer(0, &pVertex); // ���ؽ��۾��Ϸ��µ�

	D3DVERTEXELEMENT9 element[MAX_FVF_DECL_SIZE]; // MAX_FVF_DECL_SIZE ��ŭ �迭�Ҵ�
	::ZeroMemory(element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE); // element �ʱ�ȭ
	// MAX_FVF_DECL_SIZE �꺸�� +1 �Ǿ��ִµ� �װŴ� ���� �ǹ��ϴµ�

	mesh->GetDeclaration(element); // �����鿡 ���� ������ �� ������ element�� ��´�

	UCHAR offSet = 0;
	for (ULONG i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		//Usage : ������������ �����̷�
		if (element[i].Usage == D3DDECLUSAGE_POSITION) // ������ �ʴ� �������̸�? ����ƽ�̿��� �������ϴ°ǰ� 
		{
			offSet = static_cast<UCHAR>(element[i].Offset); // �������� ���
			break;
		}
	}
	stride = D3DXGetFVFVertexSize(FVF); // FVF�� ����� ������ ������?
	for (ULONG i = 0; i < numVertex; ++i)
		vertexPosition[i] = *(reinterpret_cast<D3DXVECTOR3*>((static_cast<UCHAR*>(pVertex) + (i * stride + offSet)))); // �����̰Ƕ�
		// vertexPosition�� �迭�� �ƴѵ� ��� �迭ó������?
	subset = static_cast<D3DXMATERIAL*>(subsetBuffer->GetBufferPointer()); // ���� �� �����͵� ������ �޾ƿ´�
	
	textures = new LPDIRECT3DTEXTURE9[subsetCount]; // �ӿ�
	for (ULONG i = 0; i < subsetCount; ++i)
	{
		textures[i] = nullptr;
		if (subset[i].pTextureFilename == nullptr)
			continue;
		WCHAR filename[MAX_PATH] = L"";
		::lstrcpy(finalpath, _filepath.c_str()); // ���� ��� ����
		MultiByteToWideChar(CP_ACP, 0, subset[i].pTextureFilename, strlen(subset[i].pTextureFilename), filename, MAX_PATH); // ANSI �� �����ڵ�� �ٲ۴�
		::lstrcat(finalpath, filename); // �ٿ�
		hr = D3DXCreateTextureFromFile(DEVICE,
									   finalpath, 
									   &textures[i]); // texture ���Ͽ��� ã�Ƽ� ����µ� ����ü ��θ� ��� �޾ƿ��°ž�
	}
	mesh->UnlockVertexBuffer(); // unlock
	return hr; // �ؽ�ó ����������� S_OK���� �����ϰ���?
}

void StaticMesh::RenderMesh(void) // �޽� ������
{
	for (int i = 0; i < subsetCount; ++i)
	{
		DEVICE->SetTexture(0, textures[i]);
		mesh->DrawSubset(i);
	}
}

void StaticMesh::RenderMesh(LPD3DXEFFECT& _effect) // ��� �� �������̾� ����Ʈ ������?
{
	for (int i = 0; i < subsetCount; ++i)
	{
		// ����Ʈ?
		_effect->SetTexture((D3DXHANDLE)"g_BaseTexture", textures[i]);
		_effect->CommitChanges();
		mesh->DrawSubset(i);
	}
}

Resources* StaticMesh::Clone(void) // ���� �´µ�
{
	return new StaticMesh(*this);
}

void StaticMesh::Free(void) // ����
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
