#include "AxisArrow.h"
#include"Engine/Model.h"
AxisArrow::AxisArrow(GameObject* parent):GameObject(parent,"AxisArrow")
{
	for (auto i : hModel_)
	{
		i = -1;
	}
}

void AxisArrow::Initialize()
{
	string color[3] = { "R","G","B" };
	for (int i =0;i<3;i++)
	{
		hModel_[i] = Model::Load("Assets\\arrow" + color[i] + ".fbx");
	}
}

void AxisArrow::Update()
{
}

void AxisArrow::Draw()
{
	Transform t[3];
	for (auto i : t)
	{
		i = transform_;
	}
	
	t[1].rotate_.z = 90;
	t[2].rotate_.y = -90;
	
	for (int i = 0; i < 3; i++)
	{

		Model::SetTransform(hModel_[i], t[i]);
		Model::Draw(hModel_[i]);
	}
}

void AxisArrow::Release()
{
}
