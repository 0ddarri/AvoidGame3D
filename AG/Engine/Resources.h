#pragma once
#include "Component.h"

NAMESPACE(Engine)
class Resources : public Component
{
protected:
    bool isCloned = false; 
public:
    explicit Resources(void) {};
    explicit Resources(const Resources& cp);
    virtual ~Resources(void) {};

    virtual Resources* Clone(void) PURE; // 복사 뭐 그런건가

    void Free(void) override {};
};
END
