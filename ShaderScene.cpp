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
	hModel_.push_back(Model::Load("Assets\\sphere.fbx"));

	for (auto i : hModel_)
	{
		assert(i >= 0);
	}
	Camera::SetPosition(XMFLOAT3(3, 10,-10));
	Instantiate<AxisArrow>(this);

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
	Transform qt = st_;
	qt.scale_=Transform::Float3Add(qt.scale_, XMFLOAT3{1.5f,1.5f,1.5f});
}

void ShaderScene::Release()
{
}
