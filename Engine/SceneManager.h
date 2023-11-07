#pragma once
#include "GameObject.h"

//ƒQ[ƒ€ƒV[ƒ“‚ğ—ñ‹“
enum SCENE_ID
{
    SCENE_ID_TEST = 0,
    SCENE_ID_PLAY,
};

class SceneManager :
    public GameObject
{
    SCENE_ID currentSceneID_;
    SCENE_ID nextSceneID_;
public:
    SceneManager(GameObject* parent);
    ~SceneManager() override;
    
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void ChangeScene(SCENE_ID _next);
};

