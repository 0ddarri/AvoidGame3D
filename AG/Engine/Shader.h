#pragma once
#include "Resources.h"

NAMESPACE(Engine)
class Shader final : public Resources
{
private:
    LPD3DXEFFECT effect = nullptr; // 이펙트?
    LPD3DXBUFFER errormsg = nullptr; // 에러메세지?
protected:
public:
    explicit Shader(void);
    explicit Shader(const Shader& cp);
    virtual ~Shader(void);

    void ResetDevice(void) override;
    void LostDevice(void) override;

    HRESULT CreateShader(std::wstring _filepath); // 셰이더 만들기?

    LPD3DXEFFECT GetEffect(void) { return effect; }; // 이펙트 가져오기?
    
    HRESULT SetupTable(const D3DXMATRIX& _world); // 테이블 셋업?

    Resources* Clone(void) override; // 복사넹

    void Free(void) override;
};
END
