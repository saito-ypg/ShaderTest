#include "GameObject.h"
#include"global.h"
#include"SphereCollider.h"
GameObject::GameObject() :pParent_(nullptr),IsDead(false),pCollider_(nullptr), objectName_("")
{
}

GameObject::GameObject(GameObject* parent, const std::string& name) :pParent_(parent), objectName_(name), IsDead(false), pCollider_(nullptr)
{
	if (parent)
		transform_.pParent_ = &parent->transform_;
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	this->Update();

	for (auto it = childList_.begin(), end = childList_.end(); it != end;)
	{
		(*it)->UpdateSub();
	
		if ((*it)->IsDead)
		{
			(*it)->ReleaseSub();
			SAFE_DELETE(*it);
			it = childList_.erase(it);
		}
		else
		{
			it++;
		}
	}
	RoundRobin(GetRootJob());
}

void GameObject::DrawSub()
{

	this->Draw();

	for (auto it = childList_.begin(), end = childList_.end(); it != end; it++)
	{

		(*it)->DrawSub();

	}

}

void GameObject::ReleaseSub()
{
	for (auto it : childList_)
	{
		(*it).ReleaseSub();
		SAFE_DELETE(it);
	}
	this->Release();

}

void GameObject::KillMe()
{
	IsDead = true;
}

GameObject* GameObject::FindChildObject(string _objName)
{
	if (_objName == this->objectName_)
	{
		return (this);
	}
	else
	{
		for (auto e : childList_)
		{
			GameObject* obj = e->FindChildObject(_objName);
			if (obj != nullptr)
				return obj;
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
		return this;
	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(string _objName)
{
	return GetRootJob()->FindChildObject(_objName);
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	this->pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{//Targetにコライダーがあるかと自分が対象になってないかのチェック、なってたら抜ける
	if (pTarget->pCollider_ == nullptr || pTarget == this)
	{
		return;
	}
	////球同士
	//XMVECTOR vTarget = XMLoadFloat3(&pTarget->transform_.position_);
	//XMVECTOR vOwn = XMLoadFloat3(&transform_.position_);
	//if (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius() >= XMVectorGetX(XMVector3Length(vTarget - vOwn)))
	float dist =
		  (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.x - pTarget->transform_.position_.x)
		+ (transform_.position_.y - pTarget->transform_.position_.y) * (transform_.position_.y - pTarget->transform_.position_.y)
		+ (transform_.position_.z - pTarget->transform_.position_.z) * (transform_.position_.z - pTarget->transform_.position_.z);
	float rDist = (pCollider_->GetRadius() + pTarget->pCollider_->GetRadius()) * (pCollider_->GetRadius() + pTarget->pCollider_->GetRadius());
	if(dist<=rDist)
	{
		OnCollision(pTarget);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)
	{
		return;
	}
	if (pTarget->pCollider_ != nullptr)
		Collision(pTarget);
	for (const auto& itr : pTarget->childList_)
	{
		RoundRobin(itr);
	}
}

