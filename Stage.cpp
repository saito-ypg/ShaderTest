#include "Stage.h"
#include"Engine/Model.h"

//コンストラクタ
Stage::Stage(GameObject* parent)	 :GameObject(parent, "Stage"), hModel_(-1),pSprite_(nullptr)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    pSprite_ = new Sprite();
    pSprite_->Initialize();
}

//更新
void Stage::Update()
{
    transform_.rotate_ .y+= 1.0f;
}

//描画
void Stage::Draw()
{
    pSprite_->Draw(transform_);
}

//開放
void Stage::Release()
{
    delete pSprite_;
}