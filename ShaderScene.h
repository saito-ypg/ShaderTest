#pragma once
#include "Engine/GameObject.h"
#include<vector>
class ShaderScene:public GameObject
{
    std::vector<int> hModel_;
public:
    ShaderScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

