#include "Model.h"
#include"global.h"
namespace Model{
	
	std::vector<ModelData*> modelList_;//モデルのポインタを入れるベクタ
}
int Model::Load(std::string fileName)
{
	//ファイル名のモデルを読み込み、同じモデル名があればそのモデル番号を返す
	//同じ名前のモデルがなければ、読み込んでベクタに追加
	/*for (int i = 0; i < modelList_.size(); i++)
	{
		if (modelList_.at(i)->fileName_ == fileName)
			return i;
	}*/

	ModelData* pData=new ModelData;
	pData->fileName_ = fileName;
	pData->pFbx_ = nullptr;

	for (auto &e : modelList_)
	{
		if (e->fileName_ == fileName)
		{
			pData->pFbx_ = e->pFbx_;
			break; 
		}
	}
	if (pData->pFbx_ == nullptr)
	{
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(fileName);
	}
	
	modelList_.push_back(pData);
	return modelList_.size() - 1;

}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList_.at(hModel)->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList_.at(hModel)->pFbx_->Draw(modelList_.at(hModel)->transform_);
}

void Model::Release(int hModel)
{
	
}

void Model::Release()
{
	bool isReffered = false;
	for (int i = 0; i < modelList_.size(); i++)
	{
		for (int j = i + 1; j < modelList_.size(); j++)
		{
			if (modelList_.at(i)->pFbx_ == modelList_.at(j)->pFbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (!isReffered)
		{
			SAFE_DELETE(modelList_.at(i)->pFbx_);
		}
		SAFE_DELETE(modelList_.at(i));
	}

	modelList_.clear();
}
