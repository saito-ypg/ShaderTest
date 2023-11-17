#include "ShaderScene.h"
#include"Engine/Model.h"
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
	Camera::SetPosition(XMFLOAT3(3, 10, -10));
	Instantiate<AxisArrow>(this);
}

void ShaderScene::Update()
{
}

void ShaderScene::Draw()
{
	for (auto i : hModel_)
	{
		Model::SetTransform(i, transform_);
		Model::Draw(i);
	}

}

void ShaderScene::Release()
{
}
