//픽셀 셰이더 입력
// 셰이더/쉐이더(shader)


struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

// 텍스처 / 샘플러 스테이트.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	// 텍스처 색상 읽어오기.
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// 음영처리 (디퓨즈).

	// = float3 diffuse = max(0, inputSettings)
	float3 diffuse = saturate(input.diffuse);
	diffuse = diffuse * textureColor.rgb; //텍스쳐 로드하는 부분

	// 값 정리
	float3 reflection = normalize(input.reflection);
	float3 viewDir = normalize(input.viewDir);

	//스페큘러
	float3 specular = 0;

	//빛의 강도가 0보다 큰 경우만 계산
	if (diffuse.x > 0)
	{
		float3 rDotv = dot(reflection, -viewDir);
		specular = saturate(rDotv);
		specular = pow(specular, 20.0f); //스페큘러의 강도, 빛의 세기
	}

	float3 ambient = { 0.05f, 0.05f, 0.05f };

	float3 specularColor = {1.0f, 0.0f, 0.0f };
	//float3 finalColor = (1.0f - specular) * specularColor;
	//색상으로 생각하면 절대 답 안나옴.
	//숫자로 생각한다면 0이상 되는 값이 현재 스페큘러 값이니 값을 뒤집으려면 맥스값인 1에서 빼주면 반전된 값이 나온다.

	return float4(diffuse + specular , 2.0f);
}