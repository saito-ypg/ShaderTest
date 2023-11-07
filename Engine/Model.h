#pragma once
#include<string>
#include<vector>
#include"Fbx.h"
namespace Model
{
	struct ModelData
	{
		Fbx* pFbx_;//fbx
		Transform transform_;//�g�����X�t�H�[��
		std::string fileName_;//�t�@�C����

	};
	int Load(std::string fileName);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release(int hModel);
	void Release();//���f�����g���Ă��Ȃ����m�F
	
}

