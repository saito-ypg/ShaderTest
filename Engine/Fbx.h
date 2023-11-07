#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
class Texture;
#pragma comment(lib, "LibFbxSDK-Md.lib")
#pragma comment(lib, "LibXml2-Md.lib")
#pragma comment(lib, "zlib-Md.lib")

class Fbx
{
private:
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuseColor;		// ディフューズカラー（マテリアルの色）
		int		isTexture;		// テクスチャ貼ってあるかどうか
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};
	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;//マテリアルのポインタ。後で配列になる可能性
	int* indexCount_;
public:

	Fbx();
	~Fbx();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();


private:
	void PassDataToCB(Transform transform,int i);
	void SetBufferToPipeline(int i);
	//頂点バッファ準備
	HRESULT	InitVertex(fbxsdk::FbxMesh* mesh);
	HRESULT	InitIndex(fbxsdk::FbxMesh* mesh);
	HRESULT InitConstantBuffer();	//コンスタントバッファ準備
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);
};