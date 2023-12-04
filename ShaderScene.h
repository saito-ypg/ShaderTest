#pragma once
#include "Engine/GameObject.h"
#include<vector>
class Quad;
class ShaderScene:public GameObject
{
    std::vector<int> hModel_;
    Transform st_;
    XMFLOAT3 lightpos_;
public:
    ShaderScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

