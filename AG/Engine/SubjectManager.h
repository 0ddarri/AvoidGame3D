#pragma once
#include "Subject.h"
#include "Singleton.h"
NAMESPACE(Engine)
// ???ϴ¾???
class SubjectManager final : public Subject, public Singleton<SubjectManager>
{
private:
    std::map<UINT, void*> datagroup; // ?????? ?????ϴ? map
protected:
public:
    void AddData(UINT _flag, void* _data);
    void SetData(UINT _flag, void* _data);
    void DeleteData(UINT _flag);
    
    void* GetData(UINT _flag);

    void ClearObservers(void);

    void Free(void) override {};
};
END