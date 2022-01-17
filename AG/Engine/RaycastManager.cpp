#include "DXUT.h"
#include "RaycastManager.h"
USING(Engine)
void RaycastManager::GetRay(const D3DXMATRIX& _world)
{
    const D3DSURFACE_DESC* backbuffer = DXUTGetD3D9BackBufferSurfaceDesc(); // 대충 백버퍼의 머시기를 얻어오는거같은데
    D3DXMATRIX matproj;
    DXUTGetD3D9Device()->GetTransform(D3DTS_PROJECTION, &matproj); // 투영행렬이였나 이게

    POINT point = GetMousePoint(); // 마우스 포인트 뷰포트 좌표 받아와

    D3DXVECTOR3 v; // 스크린좌표를 투영좌표? 로 변환하고 그 투영행렬의 역변환까지 하는거같은데 / 뷰포트 -> 프로젝션
    v.x = (((2.0f * point.x) / backbuffer->Width) - 1) / matproj._11;
    v.y = -(((2.0f * point.y) / backbuffer->Height) - 1) / matproj._22;
    v.z = 1.0f;
	// 자 이렇게 뷰 좌표의 x y를 찾음 z는 모르니까 임의의 값 1.0 넣어


    D3DXMATRIX matView; // 뷰행렬
    DXUTGetD3D9Device()->GetTransform(D3DTS_VIEW, &matView); // 뷰행렬 받아와
    D3DXMATRIX matWorld = _world; // 월드행렬 받아오는거겠지?
	// 우리가 보는 화면은 월드, 뷰, 프로젝션의 과정을 거쳐서 보이는거
	// 최종적으로 프로젝션 변환을 거친 화면이 보이는거니까 모니터 마우스 클릭할 때는 프로젝션 되어있는거니까
	// 나머지 월드랑 뷰를 역변환해주면 월드 공간이 나오게 된다
    D3DXMATRIX WV = matWorld * matView; // 그래서 뷰와 월드를 곱해주는듯
    D3DXMATRIX m; // 이걸로 광선의 위치랑 방향을 다 정하는건가
    D3DXMatrixInverse(&m, NULL, &WV); // 역행렬 계산해
	//https://discord.com/channels/832910749118365706/930855514101936258
	// 이걸로 광선의 방향을 정하는거같은데
    rayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
    rayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
    rayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	// 광선의 포지션인듯
    rayPos.x = m._41;
    rayPos.y = m._42;
    rayPos.z = m._43;
}

// 마우스 포인트 받아오는거
POINT RaycastManager::GetMousePoint(void)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(DXUTGetHWND(), &p);
	return p;
}

// 마우스커서로 메쉬 선택하는거같은데
bool RaycastManager::PickMeshWithMouse(out float& _distanceOut, out D3DXVECTOR3& _targetposition, in const LPD3DXMESH& _mesh, in const D3DXMATRIX& _world)
{
	GetRay(_world);
	BOOL isHit;
	DWORD face;
	FLOAT u, v, distance;
	D3DXIntersect(_mesh, &rayPos, &rayDir, &isHit, &face, &u, &v, &distance, NULL, NULL); // 광선과 메쉬가 교차하는지 확인
	if (isHit) // 충돌했으면
	{
		_distanceOut = distance;
		_targetposition = rayPos + (rayDir * distance);
		return true;
	}
	return false;
}

// 삼각형을 마우스로 선택하는듯
bool RaycastManager::PickTriWithMouse(out FLOAT& _distanceOut, out D3DXVECTOR3& _triposition, in const D3DXVECTOR3& _p0, in const D3DXVECTOR3& _p1, in const D3DXVECTOR3& _p2, in const D3DXMATRIX& _world)
{
	FLOAT u, v, distance;
	GetRay(_world);
	if (D3DXIntersectTri(&_p0, &_p1, &_p2, &rayPos, &rayDir, &u, &v, &distance)) // 광선과 삼각형의 교집합 계산? 암튼 충돌하면
	{
		return true;
	}
	return false;
}

// 방향으로 메쉬 선택? 그 바라보는 방향 그건가
bool RaycastManager::PickMeshWithDirection(out float& _distanceOut, out D3DXVECTOR3& _targetposition, in const LPD3DXMESH& _mesh, in const D3DXVECTOR3& _direction, in const D3DXVECTOR3& _position, in const D3DXMATRIX& _world)
{
	BOOL isHit;
	DWORD face;
	FLOAT u, v, distance;

	D3DXMATRIX mat = _world;
	D3DXVECTOR3 pos = _position;
	D3DXVECTOR3 dir = _direction;
	D3DXMatrixInverse(&mat, 0, &mat);
	D3DXVec3TransformCoord(&pos, &pos, &mat);
	D3DXVec3TransformNormal(&dir, &dir, &mat);

	D3DXIntersect(_mesh, &pos, &dir, &isHit, &face, &u, &v, &distance, NULL, NULL);
	if (isHit)
	{
		_distanceOut = distance;
		_targetposition = _position + (_direction * distance);
		return true;
	}
	return false;
}