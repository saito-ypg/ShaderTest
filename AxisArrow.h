#pragma once
#include "Engine/GameObject.h"
class AxisArrow :
	public GameObject
{
    int hModel_[3];
public:
	AxisArrow(GameObject*parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

