#include "Transform.h"

Transform::Transform():pParent_(nullptr)
{
	position_ = XMFLOAT3( 0,0,0 );
	rotate_ = XMFLOAT3(0,0,0 );
	scale_={ 1,1,1 };
	matTranslate_ =XMMatrixIdentity();
	matRotate_ = XMMatrixIdentity();
	matScale_ = XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
	matRotate_ = XMMatrixRotationZ(XMConvertToRadians( rotate_.z))*XMMatrixRotationX(XMConvertToRadians(rotate_.x))* XMMatrixRotationY(XMConvertToRadians(rotate_.y));
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

}

XMMATRIX Transform::GetWorldMatrix()
{
	if (pParent_)//e‚ª‘¶Ý‚·‚é‚È‚ç
	{
		return  matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}
	return matScale_*matRotate_*matTranslate_;
}

XMMATRIX Transform::GetNormalMatrix()
{

	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}
