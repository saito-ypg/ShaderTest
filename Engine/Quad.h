#pragma once
#include <DirectXMath.h>
#include"Direct3D.h"
#include "Texture.h"
#include"Transform.h"
using namespace DirectX;

//コンスタントバッファー



class Quad
{
protected:
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};
	UINT64 vertexNum_;
	VERTEX* vertices_;
	UINT64 indexNum_;//インデックス情報
	int* index_;//インデックス情報
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;   //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;//テクスチャ
public:
	Quad();
	virtual ~Quad();
	HRESULT Initialize();
	void Draw(Transform& transform);
	void Release();

private:
	//initialize内の関数
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();

	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();

	HRESULT CreateConstantBuffer();

	HRESULT LoadTexture();


	//Draw関数から呼ばれる関数
	void PassDataToCB(Transform transform);
	void SetBufferToPipeline();
};

