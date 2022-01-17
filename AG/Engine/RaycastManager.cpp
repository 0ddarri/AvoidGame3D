#include "DXUT.h"
#include "RaycastManager.h"
USING(Engine)
void RaycastManager::GetRay(const D3DXMATRIX& _world)
{
    const D3DSURFACE_DESC* backbuffer = DXUTGetD3D9BackBufferSurfaceDesc(); // ���� ������� �ӽñ⸦ �����°Ű�����
    D3DXMATRIX matproj;
    DXUTGetD3D9Device()->GetTransform(D3DTS_PROJECTION, &matproj); // ��������̿��� �̰�

    POINT point = GetMousePoint(); // ���콺 ����Ʈ ����Ʈ ��ǥ �޾ƿ�

    D3DXVECTOR3 v; // ��ũ����ǥ�� ������ǥ? �� ��ȯ�ϰ� �� ��������� ����ȯ���� �ϴ°Ű����� / ����Ʈ -> ��������
    v.x = (((2.0f * point.x) / backbuffer->Width) - 1) / matproj._11;
    v.y = -(((2.0f * point.y) / backbuffer->Height) - 1) / matproj._22;
    v.z = 1.0f;
	// �� �̷��� �� ��ǥ�� x y�� ã�� z�� �𸣴ϱ� ������ �� 1.0 �־�


    D3DXMATRIX matView; // �����
    DXUTGetD3D9Device()->GetTransform(D3DTS_VIEW, &matView); // ����� �޾ƿ�
    D3DXMATRIX matWorld = _world; // ������� �޾ƿ��°Ű���?
	// �츮�� ���� ȭ���� ����, ��, ���������� ������ ���ļ� ���̴°�
	// ���������� �������� ��ȯ�� ��ģ ȭ���� ���̴°Ŵϱ� ����� ���콺 Ŭ���� ���� �������� �Ǿ��ִ°Ŵϱ�
	// ������ ����� �並 ����ȯ���ָ� ���� ������ ������ �ȴ�
    D3DXMATRIX WV = matWorld * matView; // �׷��� ��� ���带 �����ִµ�
    D3DXMATRIX m; // �̰ɷ� ������ ��ġ�� ������ �� ���ϴ°ǰ�
    D3DXMatrixInverse(&m, NULL, &WV); // ����� �����
	//https://discord.com/channels/832910749118365706/930855514101936258
	// �̰ɷ� ������ ������ ���ϴ°Ű�����
    rayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
    rayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
    rayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	// ������ �������ε�
    rayPos.x = m._41;
    rayPos.y = m._42;
    rayPos.z = m._43;
}

// ���콺 ����Ʈ �޾ƿ��°�
POINT RaycastManager::GetMousePoint(void)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(DXUTGetHWND(), &p);
	return p;
}

// ���콺Ŀ���� �޽� �����ϴ°Ű�����
bool RaycastManager::PickMeshWithMouse(out float& _distanceOut, out D3DXVECTOR3& _targetposition, in const LPD3DXMESH& _mesh, in const D3DXMATRIX& _world)
{
	GetRay(_world);
	BOOL isHit;
	DWORD face;
	FLOAT u, v, distance;
	D3DXIntersect(_mesh, &rayPos, &rayDir, &isHit, &face, &u, &v, &distance, NULL, NULL); // ������ �޽��� �����ϴ��� Ȯ��
	if (isHit) // �浹������
	{
		_distanceOut = distance;
		_targetposition = rayPos + (rayDir * distance);
		return true;
	}
	return false;
}

// �ﰢ���� ���콺�� �����ϴµ�
bool RaycastManager::PickTriWithMouse(out FLOAT& _distanceOut, out D3DXVECTOR3& _triposition, in const D3DXVECTOR3& _p0, in const D3DXVECTOR3& _p1, in const D3DXVECTOR3& _p2, in const D3DXMATRIX& _world)
{
	FLOAT u, v, distance;
	GetRay(_world);
	if (D3DXIntersectTri(&_p0, &_p1, &_p2, &rayPos, &rayDir, &u, &v, &distance)) // ������ �ﰢ���� ������ ���? ��ư �浹�ϸ�
	{
		return true;
	}
	return false;
}

// �������� �޽� ����? �� �ٶ󺸴� ���� �װǰ�
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