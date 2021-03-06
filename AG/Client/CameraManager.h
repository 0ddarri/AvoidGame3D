#pragma once
#include "../Engine/Singleton.h"
class CameraManager : public Engine::Singleton<CameraManager>
{
private:
    std::map<CAMERAINDEX, Engine::GameObject*> cameraMap;
    Engine::GameObject* currentCamera = nullptr;
    CAMERAINDEX currentIndex{CAM_MENU};

protected:
public:
    explicit CameraManager(void) = default;
    CameraManager(const CameraManager&) = delete;
    CameraManager(CameraManager&&) = delete;

    void AddCamera(CAMERAINDEX _key, Engine::GameObject* _camera);
    void SetCamera(CAMERAINDEX _key);

    CUTSCENEINDEX GetCurrentCutScene(void) const;
    void SetCurrentCutScene(CUTSCENEINDEX _index);

    Engine::GameObject* GetCurrentCamera(void) const;
    CAMERAINDEX GetCurrentIndex(void) const;
    float GetDelta(void) const;

    void UpdateCamera(const FLOAT& dt);
    void LateUpdateCamera(const FLOAT& dt);
    void DeleteCameraDatas(void);
};

