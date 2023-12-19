//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

Texture2D g_toon_texture : register(t1);//toon�p�e�N�X�`��
//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global : register(b0)
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //���[���h�s��
    float4x4 matN; //�@���ϊ��s��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 ambientColor;
    float4 specularColor;
    float shininess;
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
};
cbuffer global : register(b1)
{
    float4 Cam; //�J�������W
    float4 light; //�������W�A��������Ƃɕ��s�����ɂ���
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i���邳�j
    float4 campos : TEXCOORD1;
    float4 normal : TEXCOORD2;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT) 0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy;

	//�@������]
    normal.w = 0;
    normal = mul(normal, matN);
    normal = normalize(normal);
    outData.normal = normal;
 

    float4 light_ = normalize(light);
    
    
    outData.color = saturate(dot(normal, light_)); //�����o�[�gdiffuse�p
    float4 posw = mul(pos, matW); //�����x�N�g��
    outData.campos = Cam - posw;
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightsourse = float4(1.0, 1.0, 1.0, 0.0);
    float4 diffuse = { 0, 0, 0, 0 };
    float4 ambient = { 0, 0, 0, 0 };

  
    float2 uv =/* { inData.color.x, 0.5f };*/
    {
      inData.color.x,
        abs(dot(normalize(inData.campos), inData.normal))
    };
    float4 toon = g_toon_texture.Sample(g_sampler, uv);
    
    if (isTexture)
    {
        diffuse = lightsourse * g_texture.Sample(g_sampler, inData.uv) * toon;
        ambient = lightsourse * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
    }
    else
    {
        diffuse = lightsourse * diffuseColor * toon;
        ambient = lightsourse * diffuseColor * ambientColor;
    }
   
    //return (diffuse + ambient);
    return toon;

    //�֊s�������x�N�g���Ɩʂ̖@���̊p�x��90�x���炢
    //if (abs(dot(normalize(inData.campos), inData.normal))<0.3)
    //    return float4(0, 0, 0, 0);
    //else
    //    return float4(1, 1, 1, 0);
    //return toon;
}