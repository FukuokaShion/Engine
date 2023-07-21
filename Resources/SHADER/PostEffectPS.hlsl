#include"PostEffect.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET{

	float offsetU = 1 / 1280.0f;
	float offsetV = 1 / 720.0f;

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	//掛ける強度
	int kernelSize = KernelSize;

	// 平均値を計算するための総和
	float4 sum = float4(0, 0, 0, 0);

	// カーネルサイズの範囲内でピクセルをイテレート
	for (int y = -kernelSize; y <= kernelSize; ++y){
		for (int x = -kernelSize; x <= kernelSize; ++x){
			float2 offset = float2(x, y) * float2(offsetU, offsetV);
			float4 colortex0 = tex0.Sample(smp, input.uv + offset);
			sum += colortex0;
		}
	}

	// 総和をカーネルサイズで割ることで平均値を計算
	float kernelArea = (2 * kernelSize + 1) * (2 * kernelSize + 1);
	float4 averageColor = sum / kernelArea;

	float4 color = averageColor;

	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = colortex0;
	}

	return color;
}
