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
	void Release(int hModel);//���g�p
	void Release();//���f�����g���Ă��Ȃ����m�F
	/// <summary>
	/// ���f���̐F�������I�ɕύX����
	/// </summary>
	/// <param name="hModel">�n���h��</param>
	/// <param name="rgba">RGBA 0~255</param>
	void SetDiffuse(int hModel, XMFLOAT4 rgba);
}

