#include "Fbx.h"
#include"Direct3D.h"
#include"Camera.h"
#include"Texture.h"

#define TOON false
Fbx::Fbx() :pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pMaterialList_(nullptr), vertexCount_(0), polygonCount_(0), materialCount_(0), indexCount_(nullptr), scrollVal(0.0f,0.0f)
{
}

Fbx::~Fbx()
{
	Release();
}

HRESULT Fbx::Load(std::string fileName)
{
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();
	if (!pFbxManager)
	{
		MessageBox(nullptr, "FBXマネージャ作製に失敗しました", "FBXエラー", MB_OK);
		return E_FAIL;
	}
	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	if (!fbxImporter)
	{
		MessageBox(nullptr, "FBXインポータ作製に失敗しました", "FBXエラー", MB_OK);
		return E_FAIL;
	}
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);//読み込み失敗
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	if (!rootNode)
	{
		MessageBox(nullptr, "FBX情報の読み取りに失敗しました", "FBXエラー", MB_OK);
		return E_FAIL;
	}
	FbxNode* pNode = rootNode->GetChild(0);
	if (!pNode)
	{
		MessageBox(nullptr, "FBXノードの読み取りに失敗しました", "FBXエラー", MB_OK);
		return E_FAIL;
	}
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();	//頂点の数
	polygonCount_ = mesh->GetPolygonCount();	//ポリゴンの数
	materialCount_ = pNode->GetMaterialCount();//マテリアルの数


	//現在のカレントディレクトリを覚えておく
	char curDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, curDir);

	//引数のfileNameからディレクトリ部分を取得
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	//カレントディレクトリ変更
	SetCurrentDirectory(dir);

	HRESULT hr;
	hr=InitVertex(mesh);		//頂点バッファ準備
	if (FAILED(hr))
	{
		MessageBox(nullptr, "頂点バッファの初期化に失敗しました", "FBXエラー", MB_OK);
		return hr;
	}
	hr=InitIndex(mesh);		//インデックスバッファ準備
	if (FAILED(hr))
	{
		MessageBox(nullptr, "インデックスバッファの初期化に失敗しました", "FBXエラー", MB_OK);
		return hr;
	}
	hr=InitConstantBuffer();	//コンスタントバッファ準備
	if (FAILED(hr))
	{
		MessageBox(nullptr, "コンスタントバッファの初期化に失敗しました", "FBXエラー", MB_OK);
		return hr;
	}
	hr=InitMaterial(pNode);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "マテリアルの読み込みに失敗しました", "FBXエラー", MB_OK);
		return hr;
	}

	//カレントディレクトリを元に戻す
	char result[_MAX_DIR + 4];
	wsprintf(result, "%s", curDir);
	SetCurrentDirectory(result);

	//マネージャ解放
	pFbxManager->Destroy();
	
#if TOON
	pToonTex_ = new Texture;
	pToonTex_->Load("Assets\\toonSlider.png");
#endif
	return S_OK;
}

void Fbx::Draw(Transform& transform)
{
	scrollVal.x +=0.0001f;//uvスクロールテスト
	scrollVal.y += 0.0003f;
#if TOON
	//Direct3D::SetShader(SHADER_ToonOutline);
	//transform.Calculation();//トランスフォームを計算
	////コンスタントバッファに情報を渡す
	//for (int i = 0; i < 2; i++) {
	//	for (int i = 0; i < materialCount_; i++)
	//	{
	//		PassDataToCB(transform, i);

	//		SetBufferToPipeline(i);

	//		//描画
	//		Direct3D::pContext_->DrawIndexed(indexCount_[i], 0, 0);
	//	}

	//	Direct3D::SetShader(SHADER_3DToon);
	//}//toonの残骸
#else//toon
	Direct3D::SetShader(SHADER_NORMALMAP);
	transform.Calculation();
	for (int i = 0; i < materialCount_; i++)
	{
		PassDataToCB(transform, i);
		SetBufferToPipeline(i);
		Direct3D::pContext_->DrawIndexed(indexCount_[i], 0, 0);
	}
#endif//toon
}

void Fbx::Release()
{
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_DELETE_ARRAY(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_DELETE_ARRAY(pMaterialList_);
	SAFE_DELETE_ARRAY(indexCount_);
}

void Fbx::SetDiffuse(XMFLOAT4 rgba)
{
	for (int i = 0; i < materialCount_; i++)
	{
		pMaterialList_[i].diffuse = rgba;
	}
}

void Fbx::Setlight(XMFLOAT3 lightpos)
{
	this->light_ = lightpos;
}


//頂点バッファ準備
HRESULT Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//頂点情報を入れる配列
	VERTEX* vertices = new VERTEX[vertexCount_];

	//全ポリゴン
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = mesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//頂点のUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//頂点の法線
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}
	for (int i = 0; i < polygonCount_; i++)//タンジェントは面の数だけある
	{
		auto a=mesh->GetElementTangentCount();
		int sIndex = mesh->GetPolygonVertexIndex(i);//i番目のポリゴンの最初の頂点を返す関数
		FbxVector4 tangent = { 0,0,0,0 };
		FbxGeometryElementTangent* Etangent = mesh->GetElementTangent(0);
		if (Etangent) {
			tangent=Etangent->GetDirectArray().GetAt(sIndex).mData;//接線データあるなら入れとく
		}
		for (int j = 0; j < 3; j++)
		{
			int index = mesh->GetPolygonVertices()[sIndex + j];
			vertices[index].tangent = { (float)tangent[0],(float)tangent[1], (float)tangent[2], 0 };
		}
		
	}
	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = vertexCount_ * sizeof(VERTEX);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	return Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);


}

//インデックスバッファ準備
HRESULT Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer*[materialCount_];
	int* index = new int[polygonCount_ * 3];
	indexCount_ = new int[materialCount_];


	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;
		
		//全ポリゴン
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{//3頂点分
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index[count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}

		}
		indexCount_[i] = count;
		
		// インデックスバッファを生成する
		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = polygonCount_ * 3 * sizeof(int);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		HRESULT hr=Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT Fbx::InitConstantBuffer()//コンスタントバッファ準備
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	return  Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);

}

void Fbx::PassDataToCB(Transform transform,int i)
{
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());
	cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
	cb.diffuseColor = pMaterialList_[i].diffuse;
	cb.ambientColor = pMaterialList_[i].ambient;
	cb.specular = pMaterialList_[i].specular;
	cb.shininess =  pMaterialList_[i].shininess;
	cb.scroll = scrollVal;
	cb.hasTexture = pMaterialList_[i].pTexture != nullptr;
	cb.hasNormalMap = pMaterialList_[i].pNormalMap != nullptr;



	Direct3D::pContext_->UpdateSubresource(pConstantBuffer_, 0, NULL, &cb, 0, 0);

	if (pMaterialList_[i].pTexture)
	{
		ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
		Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);
		ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
		Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
	}
	if (pMaterialList_[i].pNormalMap)
	{
		ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pNormalMap->GetSRV();
		Direct3D::pContext_->PSSetShaderResources(1, 1, &pSRV);
	}
#if toon
	ID3D11ShaderResourceView* pSRVtoon =pToonTex_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(1, 1, &pSRVtoon);
#endif
}

void Fbx::SetBufferToPipeline(int i)
{
	//頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにセット
		//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
}

HRESULT Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new Fbx::MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		//マテリアルの色
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pNode->GetMaterial(i);

		//色情報格納
		FbxDouble3  diffuse = pPhong->Diffuse;
		FbxDouble3  ambient = pPhong->Ambient;
		
		pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2],1.0f);
		pMaterialList_[i].ambient = XMFLOAT4((float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f);
		pMaterialList_[i].specular = XMFLOAT4{ 0,0,0,0 };
		pMaterialList_[i].shininess = 1.0f;
		if (pPhong->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxDouble3 specular = pPhong->Specular;
			pMaterialList_[i].specular = XMFLOAT4((float)specular[0], (float)specular[1], (float)specular[2], 1.0f);
			pMaterialList_[i].shininess = (float)pPhong->Shininess;
		}

		//テクスチャ情報
		{
			FbxProperty  lProperty = pPhong->FindProperty(FbxSurfaceMaterial::sDiffuse);

			//テクスチャの数数
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

			//テクスチャあり
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				string textureFilePath = textureInfo->GetRelativeFileName();

				//ファイル名+拡張だけにする
				char name[_MAX_FNAME];	//ファイル名
				char ext[_MAX_EXT];	//拡張子
				_splitpath_s(textureFilePath.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//ファイルからテクスチャ作成
				pMaterialList_[i].pTexture = new Texture;
				HRESULT hr = pMaterialList_[i].pTexture->Load(name);
				if (FAILED(hr))
				{
					
					return hr;
				}

			}
			else
			{
				pMaterialList_[i].pTexture = nullptr;
			}
		}
		{//ここから法線マップ読み取り。ほぼ通常texと同じ
			FbxProperty lProperty= pPhong->FindProperty(FbxSurfaceMaterial::sBump);
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				string textureFilePath = textureInfo->GetRelativeFileName();

				//ファイル名+拡張だけにする
				char name[_MAX_FNAME];	//ファイル名
				char ext[_MAX_EXT];	//拡張子
				_splitpath_s(textureFilePath.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//ファイルからテクスチャ作成
				pMaterialList_[i].pNormalMap = new Texture;
				HRESULT hr = pMaterialList_[i].pNormalMap->Load(name);
				if (FAILED(hr))
				{
					return hr;
				}

			}
			else
			{
				pMaterialList_[i].pNormalMap = nullptr;
			}
		}
	}
	return S_OK;
}
