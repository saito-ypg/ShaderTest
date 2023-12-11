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
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;//反射強度・色
		float shininess;//ハイライトの強さ・サイズ
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;//wvp ワールドビュープロジェクション
		XMMATRIX	matW;//w ワールド変換
		XMMATRIX	matNormal;//スケール*平行移動の逆行列
		XMFLOAT4	diffuseColor;	// マテリアルの色(FBXからとってくる）
		XMFLOAT4	ambientColor;	//環境光
		XMFLOAT4	specular;		//反射光
		//XMFLOAT4	Cam;//カメラの視点,normalize
		//XMFLOAT4	lightPosition;
		float		shininess;//ハイライトの強さ
		BOOL		isTexture;		// テクスチャ貼ってあるかどうか
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
	XMFLOAT3 light_;//ライト位置
public:

	Fbx();
	~Fbx();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
	void SetDiffuse(XMFLOAT4 rgba);
	void Setlight(XMFLOAT3 lightpos);
private:
	void PassDataToCB(Transform transform,int i);
	void SetBufferToPipeline(int i);
	//頂点バッファ準備
	HRESULT	InitVertex(fbxsdk::FbxMesh* mesh);
	HRESULT	InitIndex(fbxsdk::FbxMesh* mesh);
	HRESULT InitConstantBuffer();	//コンスタントバッファ準備
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);
};