struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
};

// 텍스처 / 샘플러 스테이트.
Texture2D warpMap : register(t0);
Texture2D diffuseMap : register(t1);
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	//input.diffuse = saturate(input.diffuse);
	float2 uv = input.texCoord;

	input.diffuse = input.diffuse * 0.5 + 0.5;

	// UV 만들기
	input.texCoord = float2(input.diffuse.x, 0.5f);
	

	// 텍스처 색상 읽어오기.
	float4 warpColor = warpMap.Sample(diffuseSampler, input.texCoord);
	float4 diffuseColor = diffuseMap.Sample(diffuseSampler, uv);

	float3 finalColor = warpColor * diffuseColor;

	// 음영 처리

	return float4(finalColor.rgb, 1.0f);
}