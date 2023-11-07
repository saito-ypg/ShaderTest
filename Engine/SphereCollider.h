#pragma once
class SphereCollider
{
	//球半径
	float radius_;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_r">Radius of Sphere</param>
	SphereCollider(float _r);
	inline float GetRadius() { return radius_; }
};

