#include "ShaderScene.h"
#include"Engine/Model.h"
#include"Engine/Image.h"
#include"Engine/Camera.h"
#include"AxisArrow.h"
#include"Engine/Input.h"
ShaderScene::ShaderScene(GameObject* parent):GameObject(parent,"ShaderScene")
{
	lightpos_ = { 3,4,1 };
}

void ShaderScene::Initialize()
{
	/*hModel_.push_back(Model::Load("Assets\\ground.fbx"));
	hModel_.push_back(Model::Load("Assets\\sphere.fbx"));
	hModel_.push_back(Model::Load("Assets\\lightSphere.fbx"));*/
	hModel_.push_back(Model::Load("Assets\\torus.fbx"));
	for (auto i : hModel_)
	{
		assert(i >= 0);
	}
	Camera::SetPosition(XMFLOAT3(3, 10,-10));
	//Instantiate<AxisArrow>(this);
	Model::SetLight(lightpos_);

}

void ShaderScene::Update()
{
	const float move = 0.1f;
	if (Input::IsKey(DIK_W))
	{
		lightpos_.z += move;
		Model::SetLight(lightpos_);
	}
	if (Input::IsKey(DIK_S))
	{
		lightpos_.z -= move;
		Model::SetLight(lightpos_);
	}
	if (Input::IsKey(DIK_A))
	{
		lightpos_.x-= move;
		Model::SetLight(lightpos_);
	}
	if (Input::IsKey(DIK_D))
	{
		lightpos_.x += move;
		Model::SetLight(lightpos_);
	}
	if (Input::IsKey(DIK_Q))
	{
		lightpos_.y -= move;
		Model::SetLight(lightpos_);
	}
	if (Input::IsKey(DIK_E))
	{
		lightpos_.y += move;
		Model::SetLight(lightpos_);
	}
	st_.rotate_.y++;
}

void ShaderScene::Draw()
{
	for (auto i : hModel_)
	{Model::SetTransform(i, transform_);}
	//Model::SetTransform(hModel_.at(1),st_);
	//Transform lightT;
	//lightT.position_ = lightpos_;
	//Model::SetTransform(hModel_.at(2), lightT);
	for (auto i : hModel_)
	{
		Model::Draw(i);
	}
	//Transform qt = st_;
	//qt.scale_=Transform::Float3Add(qt.scale_, XMFLOAT3{1.5f,1.5f,1.5f});
}

void ShaderScene::Release()
{
}
