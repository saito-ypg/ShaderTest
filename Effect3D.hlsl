//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[


//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //���[���h�s��
    float4x4 matN; //�@���ϊ��s��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 Cam; //�J�������W
    float4 light; //�������W�A��������Ƃɕ��s�����ɂ���
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i���邳�j
    //float4 campos : CAM;
    //float4 normal : NORMAL;
    float4 posW : posWorld;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy;

	//�@������]
    normal = mul(normal, matN);
    //outData.normal = normal;
    //�����x�N�g��
    //outData.campos = normalize(mul(Cam, matW) - mul(pos, matW));
	
    float4 pointlight = light - pos;
    outData.color = clamp(dot(normal, normalize(light)), 0, 1);
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightsourse = float4(1.0, 1.0, 1.0, 0.0);
    float4 ambientSourse = float4(0.15, 0.15, 0.15, 1);
    float4 diffuse = { 0, 0, 0, 0 };
    float4 ambient = { 0, 0, 0, 0 };
    //float4 specular ={ 0, 0, 0, 0 };
    //float Ks = 2.0;
    //float n = 8.0;
	
    //float4 lightW = mul(light, matW);
    //float4 R = normalize(2 * inData.normal * dot(inData.normal, lightW) - lightW);
    //float4 rvpa = pow(saturate(dot(R, inData.campos)), n);
   // specular = lightsourse * Ks * rvpa;
    if (isTexture)
    {
        diffuse = lightsourse * g_texture.Sample(g_sampler, inData.uv) * inData.color;
        ambient = lightsourse * g_texture.Sample(g_sampler, inData.uv) * ambientSourse;

    }
    else
    {
        diffuse = lightsourse * diffuseColor * inData.color;
        ambient = lightsourse * diffuseColor * ambientSourse;

    }
    return (diffuse + ambient /*+ specular*/);

}