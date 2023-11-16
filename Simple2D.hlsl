//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D	g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[


//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	matW;	//���[���h�s��
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD;	//UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�s���������
	//���[���h���W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matW);
	outData.uv = uv.xy;
	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
#define Effect 0 //0=none 1=grayscale,2=posterization

float4 PS(VS_OUT inData) : SV_Target
{
#if(Effect==0)
	{
        return g_texture.Sample(g_sampler, inData.uv);
	} 
#elif(Effect==1)
	//GlayScale
	{
        const float rBright = 0.298912, gBright = 0.586611, bBright = 0.114478;
        float4 s = g_texture.Sample(g_sampler, inData.uv);
        float g = (s.r * rBright + s.g * gBright + s.b * gBright);
        return g;
    }
#elif(Effect==2)
	{//�S�̂̐F�������������ق���������
        const float step =256;
        float4 output = floor(g_texture.Sample(g_sampler, inData.uv) * step) / step;
        return output;
    }
#endif
	
}