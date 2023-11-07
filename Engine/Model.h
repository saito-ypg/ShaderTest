#pragma once
#include<string>
#include<vector>
#include"Fbx.h"
namespace Model
{
	struct ModelData
	{
		Fbx* pFbx_;//fbx
		Transform transform_;//トランスフォーム
		std::string fileName_;//ファイル名

	};
	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release(int hModel);
	void Release();//モデルを使っていないか確認
	
}

