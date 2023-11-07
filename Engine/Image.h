#pragma once
#include<string>
#include<vector>
#include"Sprite.h"
namespace Model
{
	struct ModelData
	{
		Sprite* pSprite_;//fbx
		Transform transform_;//トランスフォーム
		std::string fileName_;//ファイル名

	};
	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release(int hModel);
	void Release();//モデルを使っていないか確認

}

