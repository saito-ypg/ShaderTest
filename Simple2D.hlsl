//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー


//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	matW;	//ワールド行列
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos    : SV_POSITION;	//位置
	float2 uv	: TEXCOORD;	//UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド行列をかけて
	//ワールド座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matW);
	outData.uv = uv.xy;
	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
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
	{//全体の色味を加味したほうがいいか
        const float step =256;
        float4 output = floor(g_texture.Sample(g_sampler, inData.uv) * step) / step;
        return output;
    }
#endif
	
}