#include "ShaderScene.h"
#include"Engine/Model.h"
#include"Engine/Image.h"
#include"Engine/Camera.h"
#include"AxisArrow.h"
#include"Engine/Input.h"
#include<d3d11.h>

namespace {
	const XMFLOAT3 DEF_LIGHT_POSITION = { 1,3,-2 };
}

void ShaderScene::InitConstantBuffer()
{//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CB_STAGESCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	if (FAILED(Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_)))
	{
		MessageBox(NULL, "コンスタントバッファの初期化に失敗しました", "Stage", MB_OK);
	}
}
ShaderScene::ShaderScene(GameObject* parent):GameObject(parent,"ShaderScene"),lightpos_(DEF_LIGHT_POSITION),pCBStageScene_(nullptr)
{

}

void ShaderScene::Initialize()
{
	hModel_.push_back(Model::Load("Assets\\lightSphere.fbx"));
	hModel_.push_back(Model::Load("Assets\\WaterSurface.fbx"));
	for (auto i : hModel_)
	{
		assert(i >= 0);
	}
	Camera::SetPosition(XMFLOAT3(3, 10,-10));

	InitConstantBuffer();
}

void ShaderScene::Update()
{
	static float move = 0.05f;
	constexpr float change = 0.05f;
	if(Input::IsKeyDown(DIK_LEFT))
	{
		move -= change;
	}
	if (Input::IsKeyDown(DIK_RIGHT))
	{
		move += change;
	}
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
	if (Input::IsKey(DIK_K))
	{
		diceT.position_.x += move;
	}if (Input::IsKey(DIK_H))
	{
		diceT.position_.x -= move;
	}
	if (Input::IsKey(DIK_U))
	{
		diceT.position_.z+= move;
	}
	if (Input::IsKey(DIK_J))
	{
		diceT.position_.z -= move;
	}

	PassDatatoStageCB();
}

void ShaderScene::Draw()
{
	transform_.rotate_.y =0;
	for (auto i : hModel_)
	{
		Model::SetTransform(i, transform_);
	}
	Transform l;
	l.position_ = lightpos_;
	Model::SetTransform(hModel_.at(0),l);
	for (auto i : hModel_)
	{
		Model::Draw(i);
	}
}

void ShaderScene::Release()
{
}
void ShaderScene::PassDatatoStageCB() {
	CB_STAGESCENE cb;
	cb.lightPosition = GetLightPos();
	XMStoreFloat4(&cb.Cam, Camera::GetPosition());
	Direct3D::pContext_->UpdateSubresource(pCBStageScene_, 0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);
}