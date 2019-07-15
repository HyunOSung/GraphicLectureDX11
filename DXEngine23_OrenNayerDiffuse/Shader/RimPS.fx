struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 normal : TEXCOORD3;
};

// 텍스처 / 샘플러 스테이트.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	// 텍스처 색상 읽어오기.
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	//float3


	float3 normal = normalize(input.normal);
	float3 rimColor = float3(1.0f, 1.0f, 1.0f);
	float3 rim = abs(dot(-input.viewDir, normal));
	

	float3 rimConstant = 5.0f; //
	rim = pow(1.0f - rim, rimConstant) * rimColor;

	return float4(rim, 1.0f);
}