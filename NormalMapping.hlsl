//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

Texture2D normalMap : register(t1); //�m�[�}���}�b�v
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
    bool hasTexture; // �e�N�X�`���\���Ă��邩�ǂ���
    bool hasNormalMap; //�m�[�}���}�b�v���邩�ǂ���
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
    float4 campos : POSITION;
    float4 Neyev : POSITION1; //�m�[�}���}�b�v�p�ڋ�Ԃɕϊ����ꂽ�����x�N�g��
    float4 normal : POSITION2;
    float4 light : POSITION3;
    float4 color : POSITION4; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT) 0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy;
   
    float4 binormal = float4(cross(normal.xyz, tangent.xyz), 0);
	//�@������]
    normal.w = 0;
    normal = mul(normal, matN);
    normal = normalize(normal);
    //normal.w = 0;
    outData.normal = normal;
    
    //�ڐ��x�N�g���ϊ�
    tangent.w = 0;
    tangent = mul(tangent, matN);
    tangent = normalize(tangent);
    
    binormal = mul(binormal, matN);
    binormal = normalize(binormal);
    
    float4 posw = mul(pos, matW); //�����x�N�g��
    outData.campos = normalize(Cam - posw);
    outData.Neyev.x = dot(outData.campos, tangent);
    outData.Neyev.y = dot(outData.campos, binormal);
    outData.Neyev.z = dot(outData.campos, normal);
    outData.Neyev.w = 0;
    
    float4 light_ = normalize(light);
    light_.w = 0;
    outData.color = mul(light_, normal);
    outData.color.w = 0;
    outData.light.x = dot(light_, tangent);
    outData.light.y = dot(light_, binormal);
    outData.light.z = dot(light_, normal);
    outData.light.w = 0;
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
    float4 specular = { 0, 0, 0, 0 };
    float alpha=1.0f;
    float4 result;
    if (hasNormalMap)
    {
        inData.light = normalize(inData.light);
        float4 specular;

        float4 tmpNormal = normalMap.Sample(g_sampler, inData.uv) * 2 - 1; //�F����x�N�g����
        tmpNormal.w = 0;
        tmpNormal = normalize(tmpNormal);
    
        float4 S = saturate(dot(tmpNormal, normalize(inData.light)));
        float4 R = reflect(-inData.light, tmpNormal);
        specular = pow(saturate(dot(R, inData.Neyev)), shininess) * specularColor;
        if (hasTexture)
        {
            diffuse = lightsourse * g_texture.Sample(g_sampler, inData.uv) * S;
            ambient = lightsourse * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
           // alpha = g_texture.Sample(g_sampler, inData.uv).a;
        }
        else
        {
            diffuse = lightsourse * diffuseColor * S;
            ambient = lightsourse * diffuseColor * ambientColor;
        }
        result = diffuse + ambient + specular;
    }
    else
    {
        float4 ref = reflect(normalize(-light), inData.normal);
        specular = pow(saturate(dot(ref, normalize(inData.campos))), shininess) * specularColor;
    
        if (hasTexture)
        {
            diffuse = lightsourse * g_texture.Sample(g_sampler, inData.uv) * inData.color;
            ambient = lightsourse * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
            alpha = g_texture.Sample(g_sampler, inData.uv).a;
        }
        else
        {
            diffuse = lightsourse * diffuseColor * inData.color;
            ambient = lightsourse * diffuseColor * ambientColor;
        }
        result = diffuse + ambient + specular;
    }
    result.a = alpha;
    return result;
}