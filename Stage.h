#pragma once
#include "Engine/GameObject.h"
#include"Engine/Sprite.h"
class Stage : public GameObject
{
    int hModel_;    //モデル番号 
    Sprite *pSprite_;
public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};