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
	//�}�l�[�W���𐶐�
	FbxManager* pFbxManager = FbxManager::Create();
	if (!pFbxManager)
	{
		MessageBox(nullptr, "FBX�}�l�[�W���쐻�Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return E_FAIL;
	}
	//�C���|�[�^�[�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	if (!fbxImporter)
	{
		MessageBox(nullptr, "FBX�C���|�[�^�쐻�Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return E_FAIL;
	}
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//�V�[���I�u�W�F�N�g��FBX�t�@�C���̏��𗬂�����
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);//�ǂݍ��ݎ��s
	fbxImporter->Destroy();

	//���b�V�������擾
	FbxNode* rootNode = pFbxScene->GetRootNode();
	if (!rootNode)
	{
		MessageBox(nullptr, "FBX���̓ǂݎ��Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return E_FAIL;
	}
	FbxNode* pNode = rootNode->GetChild(0);
	if (!pNode)
	{
		MessageBox(nullptr, "FBX�m�[�h�̓ǂݎ��Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return E_FAIL;
	}
	FbxMesh* mesh = pNode->GetMesh();

	//�e���̌����擾
	vertexCount_ = mesh->GetControlPointsCount();	//���_�̐�
	polygonCount_ = mesh->GetPolygonCount();	//�|���S���̐�
	materialCount_ = pNode->GetMaterialCount();//�}�e���A���̐�


	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char curDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, curDir);

	//������fileName����f�B���N�g���������擾
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	//�J�����g�f�B���N�g���ύX
	SetCurrentDirectory(dir);

	HRESULT hr;
	hr=InitVertex(mesh);		//���_�o�b�t�@����
	if (FAILED(hr))
	{
		MessageBox(nullptr, "���_�o�b�t�@�̏������Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return hr;
	}
	hr=InitIndex(mesh);		//�C���f�b�N�X�o�b�t�@����
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̏������Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return hr;
	}
	hr=InitConstantBuffer();	//�R���X�^���g�o�b�t�@����
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�̏������Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return hr;
	}
	hr=InitMaterial(pNode);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "�}�e���A���̓ǂݍ��݂Ɏ��s���܂���", "FBX�G���[", MB_OK);
		return hr;
	}

	//�J�����g�f�B���N�g�������ɖ߂�
	char result[_MAX_DIR + 4];
	wsprintf(result, "%s", curDir);
	SetCurrentDirectory(result);

	//�}�l�[�W�����
	pFbxManager->Destroy();
	
#if TOON
	pToonTex_ = new Texture;
	pToonTex_->Load("Assets\\toonSlider.png");
#endif
	return S_OK;
}

void Fbx::Draw(Transform& transform)
{
	scrollVal.x +=0.0001f;//uv�X�N���[���e�X�g
	scrollVal.y += 0.0003f;
#if TOON
	//Direct3D::SetShader(SHADER_ToonOutline);
	//transform.Calculation();//�g�����X�t�H�[�����v�Z
	////�R���X�^���g�o�b�t�@�ɏ���n��
	//for (int i = 0; i < 2; i++) {
	//	for (int i = 0; i < materialCount_; i++)
	//	{
	//		PassDataToCB(transform, i);

	//		SetBufferToPipeline(i);

	//		//�`��
	//		Direct3D::pContext_->DrawIndexed(indexCount_[i], 0, 0);
	//	}

	//	Direct3D::SetShader(SHADER_3DToon);
	//}//toon�̎c�[
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


//���_�o�b�t�@����
HRESULT Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//���_��������z��
	VERTEX* vertices = new VERTEX[vertexCount_];

	//�S�|���S��
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3���_��
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//���ׂ钸�_�̔ԍ�
			int index = mesh->GetPolygonVertex(poly, vertex);

			//���_�̈ʒu
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//���_��UV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//���_�̖@��
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//���Ԗڂ̃|���S���́A���Ԗڂ̒��_�̖@�����Q�b�g
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}
	for (int i = 0; i < polygonCount_; i++)//�^���W�F���g�͖ʂ̐���������
	{
		auto a=mesh->GetElementTangentCount();
		int sIndex = mesh->GetPolygonVertexIndex(i);//i�Ԗڂ̃|���S���̍ŏ��̒��_��Ԃ��֐�
		FbxVector4 tangent = { 0,0,0,0 };
		FbxGeometryElementTangent* Etangent = mesh->GetElementTangent(0);
		if (Etangent) {
			tangent=Etangent->GetDirectArray().GetAt(sIndex).mData;//�ڐ��f�[�^����Ȃ����Ƃ�
		}
		for (int j = 0; j < 3; j++)
		{
			int index = mesh->GetPolygonVertices()[sIndex + j];
			vertices[index].tangent = { (float)tangent[0],(float)tangent[1], (float)tangent[2], 0 };
		}
		
	}
	// ���_�o�b�t�@�쐬
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

//�C���f�b�N�X�o�b�t�@����
HRESULT Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer*[materialCount_];
	int* index = new int[polygonCount_ * 3];
	indexCount_ = new int[materialCount_];


	for (int i = 0; i < materialCount_; i++)
	{
		int count = 0;
		
		//�S�|���S��
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{//3���_��
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index[count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}

		}
		indexCount_[i] = count;
		
		// �C���f�b�N�X�o�b�t�@�𐶐�����
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

HRESULT Fbx::InitConstantBuffer()//�R���X�^���g�o�b�t�@����
{
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
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
	//���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�R���X�^���g�o�b�t�@���p�C�v���C���ɃZ�b�g
		//���_�o�b�t�@
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p
}

HRESULT Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new Fbx::MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		//�}�e���A���̐F
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pNode->GetMaterial(i);

		//�F���i�[
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

		//�e�N�X�`�����
		{
			FbxProperty  lProperty = pPhong->FindProperty(FbxSurfaceMaterial::sDiffuse);

			//�e�N�X�`���̐���
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

			//�e�N�X�`������
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				string textureFilePath = textureInfo->GetRelativeFileName();

				//�t�@�C����+�g�������ɂ���
				char name[_MAX_FNAME];	//�t�@�C����
				char ext[_MAX_EXT];	//�g���q
				_splitpath_s(textureFilePath.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//�t�@�C������e�N�X�`���쐬
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
		{//��������@���}�b�v�ǂݎ��B�قڒʏ�tex�Ɠ���
			FbxProperty lProperty= pPhong->FindProperty(FbxSurfaceMaterial::sBump);
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
			if (fileTextureCount > 0)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				string textureFilePath = textureInfo->GetRelativeFileName();

				//�t�@�C����+�g�������ɂ���
				char name[_MAX_FNAME];	//�t�@�C����
				char ext[_MAX_EXT];	//�g���q
				_splitpath_s(textureFilePath.c_str(), nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//�t�@�C������e�N�X�`���쐬
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
