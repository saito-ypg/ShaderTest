//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー


//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
	float4x4	matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	matW;			//ワールド行列
    float4x4    matN;           //法線変換行列
	float4		diffuseColor;		// ディフューズカラー（マテリアルの色）
    float4      ambientColor;
    float4      specularColor;
    bool		isTexture; // テクスチャ貼ってあるかどうか
    float       shininess;
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
	float4 pos		: SV_POSITION;	//位置
	float2 uv		: TEXCOORD;	//UV座標
	float4 color	: COLOR;	//色（明るさ）
    float4 campos   : TEXCOORD1;
    float4 normal   : TEXCOORD2;
    float4 light    : TEXCOORD3;
    //float4 posW : posWorld;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = uv.xy;

	//法線を回転
    normal.w = 0;
	normal =mul(normal,matN);
    normal = normalize(normal);
    outData.normal = normal;
 

    float4 light_ = normalize(light);
    outData.light = normalize(light_);
    
    
    
    outData.color = saturate(dot(normal, light_));//ランバートdiffuse用
    float4 posw = mul(pos, matW); //視線ベクトル
    outData.campos = Cam - posw;
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
    float n = 8.0;
	
    float4 NL = dot(inData.light,inData.normal);
    float4 R = normalize(2 * NL * inData.normal - inData.light);
    specular = pow(saturate(dot(R, normalize(inData.campos))),n)*specularColor;
    
    

	if (isTexture)
	{
		diffuse = lightsourse * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		ambient = lightsourse * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
    }
    else
    {
        diffuse = lightsourse * diffuseColor * inData.color;
        ambient = lightsourse * diffuseColor * ambientColor;
    }
    return (diffuse + ambient + specular);

}