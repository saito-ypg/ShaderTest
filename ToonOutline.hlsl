//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

Texture2D g_toon_texture : register(t1); //toon用テクスチャ
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
    bool isTexture; // テクスチャ貼ってあるかどうか
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
    float4 color : COLOR; //色（明るさ）
    float4 campos : TEXCOORD1;
    float4 normal : TEXCOORD2;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData = (VS_OUT) 0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    normal.w = 0;
    pos = pos+normal*0.03;
    outData.pos = mul(pos, matWVP);
    outData.uv = uv.xy;

	//法線を回転
    normal = mul(normal, matN);
    normal = normalize(normal);
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    return (0, 0, 0, 1);    
}