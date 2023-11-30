#include "ShaderScene.h"
#include"Engine/Model.h"
#include"Engine/Image.h"
#include"Engine/Camera.h"
#include"AxisArrow.h"
ShaderScene::ShaderScene(GameObject* parent):GameObject(parent,"ShaderScene")
{

}

void ShaderScene::Initialize()
{
	hModel_.push_back(Model::Load("Assets\\ground.fbx"));
	//hModel_.push_back(Model::Load("Assets\\sphere.fbx"));
	hModel_.push_back(Model::Load("Assets\\tile.fbx"));
	for (auto i : hModel_)
	{
		assert(i >= 0);
	}
	Camera::SetPosition(XMFLOAT3(-3, 10, -10));
	//Instantiate<AxisArrow>(this);
	st_.position_.y += 2;
	st_.scale_ = { 5,5,5 };
}

void ShaderScene::Update()
{
	st_.rotate_.y++;
}

void ShaderScene::Draw()
{
	Model::SetTransform(hModel_.at(0), transform_);
	Model::SetTransform(hModel_.at(1),st_);
	for (auto i : hModel_)
	{
		Model::Draw(i);
	}
	
}

void ShaderScene::Release()
{
}
