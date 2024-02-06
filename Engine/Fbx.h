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
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		Texture* pNormalMap;
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;//���ˋ��x�E�F
		float shininess;//�n�C���C�g�̋����E�T�C�Y
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;//wvp ���[���h�r���[�v���W�F�N�V����
		XMMATRIX	matW;//w ���[���h�ϊ�
		XMMATRIX	matNormal;//�X�P�[��*���s�ړ��̋t�s��
		XMFLOAT4	diffuseColor;	// �}�e���A���̐F(FBX����Ƃ��Ă���j
		XMFLOAT4	ambientColor;	//����
		XMFLOAT4	specular;		//���ˌ�
		float		shininess;//�n�C���C�g�̋���
		float		scroll;
		BOOL		hasTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
		BOOL		hasNormalMap;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
		XMVECTOR tangent;//�ڐ�
	};
	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;//�}�e���A���̃|�C���^�B��Ŕz��ɂȂ�\��
	int* indexCount_;
	XMFLOAT3 light_;//���C�g�ʒu
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
	//���_�o�b�t�@����
	HRESULT	InitVertex(fbxsdk::FbxMesh* mesh);
	HRESULT	InitIndex(fbxsdk::FbxMesh* mesh);
	HRESULT InitConstantBuffer();	//�R���X�^���g�o�b�t�@����
	HRESULT InitMaterial(fbxsdk::FbxNode* pNode);
};