#include "ShaderScene.h"
#include"Engine/Model.h"
#include"Engine/Image.h"
#include"Engine/Camera.h"
#include"AxisArrow.h"
#include"Engine/Input.h"
#include<d3d11.h>

namespace {
	const XMFLOAT3 DEF_LIGHT_POSITION = { 0,0,-2 };
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
	//hModel_.push_back(Model::Load("Assets\\ground.fbx"));
	//hModel_.push_back(Model::Load("Assets\\sphere.fbx"));
	hModel_.push_back(Model::Load("Assets\\lightSphere.fbx"));
	hModel_.push_back(Model::Load("Assets\\torus.fbx"));
	hModel_.push_back(Model::Load("Assets\\TransparentDice.fbx"));
	//hModel_.push_back(Model::Load("Assets\\WaterSurface.fbx"));
	for (auto i : hModel_)
	{
		assert(i >= 0);
	}
	Camera::SetPosition(XMFLOAT3(3, 10,-10));
	Instantiate<AxisArrow>(this);
	Model::SetLight(lightpos_);
	diceT.position_ = { 3,5,-2 };


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
	for (auto i : hModel_)
	{
		Model::SetTransform(i, transform_);
	}

	Model::SetTransform(hModel_.at(1),st_);
	Transform lightT;
	lightT.position_ = lightpos_;
	Model::SetTransform(hModel_.at(0), lightT);
	
	Model::SetTransform(hModel_.at(2), diceT);
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
void ShaderScene::PassDatatoStageCB() {
	CB_STAGESCENE cb;
	cb.lightPosition = GetLightPos();
	XMStoreFloat4(&cb.Cam, Camera::GetPosition());
	Direct3D::pContext_->UpdateSubresource(pCBStageScene_, 0, NULL, &cb, 0, 0);
	//D3D11_MAPPED_SUBRESOURCE pdata;
	//Direct3D::pContext_->Map(pCBStageScene_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	//Direct3D::pContext_->Unmap(pCBStageScene_, 0);	//再開
	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);
}