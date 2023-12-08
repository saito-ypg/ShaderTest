#pragma once
#include "Engine/GameObject.h"
#include<vector>
#include<d3d11.h>
class Quad;
struct CB_STAGESCENE
{
    XMFLOAT4	Cam;//ÉJÉÅÉâÇÃéãì_,normalize
    XMFLOAT4	lightPosition;
};
class ShaderScene:public GameObject
{
    std::vector<int> hModel_;
    Transform st_;
    XMFLOAT3 lightpos_;
    void InitConstantBuffer();
    void PassDatatoStageCB();
    ID3D11Buffer *pCBStageScene_;
public:
    ShaderScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetLightpos(XMFLOAT3& _pos) { lightpos_ = _pos; };
    XMFLOAT4 GetLightPos() { return XMFLOAT4(lightpos_.x, lightpos_.y, lightpos_.z, 0); }
};

