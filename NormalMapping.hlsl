//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

Texture2D normalMap : register(t1); //ノーマルマップ
//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //ワールド行列
    float4x4 matN; //法線変換行列
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）
    float4 ambientColor;
    float4 specularColor;
    float shininess;
    bool hasTexture; // テクスチャ貼ってあるかどうか
    bool hasNormalMap; //ノーマルマップあるかどうか
};
cbuffer global : register(b1)
{
    float4 Cam; //カメラ座標
    float4 light; //光源座標、これをもとに平行光源にする
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 campos : POSITION;
    float4 Neyev : POSITION1; //ノーマルマップ用接空間に変換された視線ベクトル
    float4 normal : POSITION2;
    float4 light : POSITION3;
    float4 color : POSITION4; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData = (VS_OUT) 0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy;
   
    float4 binormal = float4(cross(normal.xyz, tangent.xyz), 0);
	//法線を回転
    normal.w = 0;
    normal = mul(normal, matN);
    normal = normalize(normal);
    //normal.w = 0;
    outData.normal = normal;
    
    //接線ベクトル変換
    tangent.w = 0;
    tangent = mul(tangent, matN);
    tangent = normalize(tangent);
    
    binormal = mul(binormal, matN);
    binormal = normalize(binormal);
    
    float4 posw = mul(pos, matW); //視線ベクトル
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
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
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

        float4 tmpNormal = normalMap.Sample(g_sampler, inData.uv) * 2 - 1; //色からベクトルに
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