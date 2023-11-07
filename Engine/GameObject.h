#pragma once
#include<string>
#include<list>
#include "Transform.h"

class SphereCollider;
using std::string;
class GameObject
{
protected:
	std::list<GameObject*> childList_;
	Transform transform_;
	GameObject* pParent_;
	std::string	objectName_;
	SphereCollider* pCollider_;
public:
	GameObject();
	GameObject(GameObject* parent, const string& name);
	virtual ~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;

	//自身と子全てのUpdateを呼ぶ
	void UpdateSub();
	//自身と子全てのDrawを呼ぶ
	void DrawSub();
	//自身と子全てのReleaseを呼ぶ
	void ReleaseSub();

	//フラグ類
	//死んでるかどうか
	bool IsDead;


	void KillMe();

	/// <summary>
	///再帰的に子から引数の名前を持つオブジェクトを探す
	/// </summary>
	/// <param name="_objName">探す名前</param>
	/// <returns>存在するならそのアドレス、なければnullptr</returns>
	GameObject* FindChildObject(string _objName);
	/// <summary>
	/// 再起呼び出しでRootJobを探す
	/// </summary>
	/// <returns>RootJobのアドレス</returns>
	GameObject* GetRootJob();
	/// <summary>
	/// 全てのオブジェクトから引数の名前を持つオブジェクトを探す
	/// </summary>
	/// <param name="_objName">探す名前</param>
	/// <returns>存在するならそのアドレス、なければnullptr</returns>
	GameObject* FindObject(string _objName);


	//アクセス関数
	GameObject* GetParent() const{ return pParent_; }
	XMFLOAT3 GetPosition() const { return transform_.position_; }
	XMFLOAT3 GetRotate() const { return transform_.rotate_; }
	XMFLOAT3 GetScale() const { return transform_.scale_; }
	XMFLOAT3 GetWorldPosition()const { return Transform::Float3Add(GetParent()->transform_.position_, transform_.position_); }
	XMFLOAT3 GetWorldRotate() const { return Transform::Float3Add(GetParent()->transform_.rotate_, transform_.rotate_); }
	XMFLOAT3 GetWorldScale() const { return Transform::Float3Add(GetParent()->transform_.scale_, transform_.scale_); }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3(x, y, z)); }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { SetRotate(XMFLOAT3(x, y, z)); }
	void SetRotateX(float x) { SetRotate(x, transform_.rotate_.y, transform_.rotate_.z); }
	void SetRotateY(float y) { SetRotate(transform_.rotate_.x, y, transform_.rotate_.z); }
	void SetRotateZ(float z) { SetRotate(transform_.rotate_.x, transform_.rotate_.y, z); }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	void SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x, y, z)); }


	//当たり判定関連 
	
	/// <summary>
	/// GameObjectに当たり判定を追加
	/// </summary>
	/// <param name="pCollider">追加するCollider</param>
	void AddCollider(SphereCollider* pCollider);
	/// <summary>
	/// 自身のColliderと対象とが衝突しているかどうか検知
	/// </summary>
	/// <param name="pTarget">対象オブジェクト</param>
	void Collision(GameObject* pTarget);
	/// <summary>
	/// 全てのコライダーと判定する
	/// </summary>
	/// <param name="pTarget">対象オブジェクト</param>
	void RoundRobin(GameObject* pTarget);

	/// <summary>
	/// Collitionで引っかかったら行う処理
	/// </summary>
	/// <param name="pTarget"></param>
	virtual void OnCollision(GameObject* pTarget) {};

	//実体化のテンプレート
	template <class T>
	T* Instantiate(GameObject* parent) {
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);
		return p;
	}

};


