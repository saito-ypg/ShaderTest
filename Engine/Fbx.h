#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#pragma warning(push)
	#pragma warning( disable : 4099 )
	#pragma comment(lib, "LibFbxSDK-Md.lib")
	#pragma comment(lib, "LibXml2-Md.lib")
	#pragma comment(lib, "zlib-Md.lib")
#pragma warning(pop)

class Texture;
class Fbx
{
private:
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		Texture* pNormalMap;
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
		float		shininess;//ハイライトの強さ
		float		scroll;
		BOOL		hasTexture;		// テクスチャ貼ってあるかどうか
		BOOL		hasNormalMap;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
		XMVECTOR tangent;//接線
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
	Texture* pToonTex_;
public:

	Fbx();
	~Fbx();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
	void SetDiffuse(XMFLOAT4 rgba);
	void Setlight(XMFLOAT3 lightpos);
	float scrollVal;
private:
	void PassDataToCB(Transform transform,int i);
	void SetBufferToPipeline(int i);
	//頂点バッファ準備
	HRESULT	InitVertex(fbxsdk::FbxMesh* mesh);
	HRESULT	InitIndex(fbxsdk::FbxMesh* mesh);
	HRESULT InitConstantBuffer();	//コンスタントバッファ準備
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);
};