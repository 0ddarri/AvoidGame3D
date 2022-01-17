#pragma once
#include "Component.h"
NAMESPACE(Engine)
class Texture;

// Unity Trail Renderer�ε�?
class Trail final : public Component 
{
private:
	struct TrailData
	{
		D3DXVECTOR3 position[2]; // ��ġ��ǥ 2���ֳ�
		double timecount = 0.0; // �ð��̳� �� ���ñ� �ð��������� Ʈ���� ��ȭ��Ű�� �׷��� �Ϸ��� �׷��°ǰ�
		TrailData(const D3DXVECTOR3& upposition, const D3DXVECTOR3& downposition)
			: timecount(0.0)
		{
			position[0] = upposition;
			position[1] = downposition;
		}

	};

private:
	ULONG vtxSize = 0; // ���ؽ� ũ��
	ULONG maxvtxCnt = 0; // �ִ� ���ؽ� ����
	ULONG maxtriCnt = 0; // �ִ� �ﰢ�� ����?
	ULONG curTriCnt = 0; // ���� �ﰢ�� ����?
	ULONG curVtxCnt = 0; // ���� ���ؽ� ����?
	double duration = 0.0; // ���ӽð�?
	double alivetime = 0.0; // ������Ÿ��
	size_t lerpcnt = 0; // ���� ī��Ʈ? �󸶳� �������� �� �׷��ǰ�
	float timer = 0.0f; // 
	float uvRate = 0.0f; // 
	std::vector<TrailData> trailDatas; // Ʈ���� �����͵�?

	LPDIRECT3DVERTEXBUFFER9 vb = nullptr; // ���ؽ�����
	LPDIRECT3DINDEXBUFFER9  ib = nullptr; // �ε�������

	Texture* trailTexture = nullptr; // Ʈ���� �ؽ�ó

public:
	explicit Trail() {};
	virtual ~Trail() {};

public:
	D3DXMATRIX* worldMatrix;
	HRESULT Initalize(const D3DXMATRIX* _worldMatrix, const ULONG& _bufferSize, const double& _duration, const double& _alivetime, const size_t& _lerpCnt, std::wstring _texturetag);
	void AddNewTrail(const D3DXVECTOR3& _upposition, const D3DXVECTOR3& _downposition, const FLOAT& _dt);
	void SplineTrailPosition(VTXTEX* _vtx, const size_t& _dataindex, ULONG& _index);
	void Update(const FLOAT& dt) override;
	void Render(const FLOAT& dt) override;

	size_t GetDataSize(void) const;
	void ClearData(void);

	void Free(void) override;
};
END