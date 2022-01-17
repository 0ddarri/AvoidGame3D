#pragma once
#include "Component.h"
NAMESPACE(Engine)
class Texture;

// Unity Trail Renderer인듯?
class Trail final : public Component 
{
private:
	struct TrailData
	{
		D3DXVECTOR3 position[2]; // 위치좌표 2개있네
		double timecount = 0.0; // 시간이네 그 뭐시기 시간지날수록 트레일 변화시키는 그런거 하려고 그러는건가
		TrailData(const D3DXVECTOR3& upposition, const D3DXVECTOR3& downposition)
			: timecount(0.0)
		{
			position[0] = upposition;
			position[1] = downposition;
		}

	};

private:
	ULONG vtxSize = 0; // 버텍스 크기
	ULONG maxvtxCnt = 0; // 최대 버텍스 개수
	ULONG maxtriCnt = 0; // 최대 삼각형 개수?
	ULONG curTriCnt = 0; // 현재 삼각형 개수?
	ULONG curVtxCnt = 0; // 현재 버텍스 개수?
	double duration = 0.0; // 지속시간?
	double alivetime = 0.0; // 라이프타임
	size_t lerpcnt = 0; // 러프 카운트? 얼마나 러프할지 뭐 그런건가
	float timer = 0.0f; // 
	float uvRate = 0.0f; // 
	std::vector<TrailData> trailDatas; // 트레일 데이터들?

	LPDIRECT3DVERTEXBUFFER9 vb = nullptr; // 버텍스버퍼
	LPDIRECT3DINDEXBUFFER9  ib = nullptr; // 인덱스버퍼

	Texture* trailTexture = nullptr; // 트레일 텍스처

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