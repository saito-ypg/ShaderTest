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
	void Release(int hModel);//未使用
	void Release();//モデルを使っていないか確認
	/// <summary>
	/// モデルの色を強制的に変更する
	/// </summary>
	/// <param name="hModel">ハンドル</param>
	/// <param name="rgba">RGBA 0~255</param>
	void SetDiffuse(int hModel, XMFLOAT4 rgba);
}

