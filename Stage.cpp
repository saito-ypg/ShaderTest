#include "Stage.h"
#include"Engine/Model.h"
#include"Engine/Image.h"
//�R���X�g���N�^
Stage::Stage(GameObject* parent)	 :GameObject(parent, "Stage"), hPict_(-1)//,pSprite_(nullptr)
{
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
  /*  pSprite_ = new Sprite();
    pSprite_->Initialize("Assets\\sample.png");*/
    hPict_=Image::Load("Assets\\sample.png");
    assert(hPict_ >= 0);
}

//�X�V
void Stage::Update()
{
  //  transform_.rotate_ .y+= 1.0f;
}

//�`��
void Stage::Draw()
{
    Image::SetTransform(hPict_, transform_);
    Image::Draw(hPict_);
}

//�J��
void Stage::Release()
{

}