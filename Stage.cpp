#include "Stage.h"
#include"Engine/Model.h"
#include"Engine/Image.h"
//コンストラクタ
Stage::Stage(GameObject* parent)	 :GameObject(parent, "Stage"), hPict_(-1)//,pSprite_(nullptr)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
  /*  pSprite_ = new Sprite();
    pSprite_->Initialize("Assets\\sample.png");*/
    hPict_=Image::Load("Assets\\sample.png");
    assert(hPict_ >= 0);
}

//更新
void Stage::Update()
{
  //  transform_.rotate_ .y+= 1.0f;
}

//描画
void Stage::Draw()
{
    Image::SetTransform(hPict_, transform_);
    Image::Draw(hPict_);
}

//開放
void Stage::Release()
{

}