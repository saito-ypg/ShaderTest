#include "Stage.h"
#include"Engine/Model.h"

//�R���X�g���N�^
Stage::Stage(GameObject* parent)	 :GameObject(parent, "Stage"), hModel_(-1),pSprite_(nullptr)
{
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
    pSprite_ = new Sprite();
    pSprite_->Initialize();
}

//�X�V
void Stage::Update()
{
    transform_.rotate_ .y+= 1.0f;
}

//�`��
void Stage::Draw()
{
    pSprite_->Draw(transform_);
}

//�J��
void Stage::Release()
{
    delete pSprite_;
}