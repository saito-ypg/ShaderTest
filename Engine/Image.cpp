#include "Image.h"
#include"global.h"
namespace Image {
    std::vector<ImageData*> imageList_;
}
int Image::Load(std::string fileName)
{
	ImageData* pData = new ImageData;
	pData->fileName_ = fileName;
	pData->pSprite_ = nullptr;

	for (auto& e : imageList_)
	{
		if (e->fileName_ == fileName)
		{
			pData->pSprite_ = e->pSprite_;
			break;
		}
	}
	if (pData->pSprite_ == nullptr)
	{
		pData->pSprite_ = new Sprite;
		pData->pSprite_->Initialize(fileName);
	}

	imageList_.push_back(pData);
	return imageList_.size() - 1;
}

void Image::SetTransform(int hPict, Transform transform)
{
	imageList_.at(hPict)->transform_ = transform;
}

void Image::Draw(int hPict)
{
	imageList_.at(hPict)->pSprite_->Draw(imageList_.at(hPict)->transform_);
}

void Image::Release(int hPict)
{
}

void Image::Release()
{
}
