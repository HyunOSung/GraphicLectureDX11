struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;

	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;

	float3 normal : TEXCOORD3;
	float3 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
};

Texture2D diffuseMap :  register(t0);
Texture2D normalMap : register(t1);
SamplerState diffuseSampler;

//메인 함수
float4 main(ps_input input) : SV_TARGET
{
	//변환 행렬
	float3x3 TBN = float3x3(
		normalize(input.tangent),
		normalize(input.binormal),
		normalize(input.normal)
	);

	float3 tangentNormal = normalMap.Sample(diffuseSampler, input.texCoord).rgb;
	tangentNormal = tangentNormal * 2 - 1;

	//공간 변환
	float3 worldNormal = mul(tangentNormal, transpose(TBN));
	worldNormal = normalize(worldNormal);

	//난반사(디퓨즈, Diffuse) 항
	float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
	float3 lightDir = normalize(input.lightDir);
	float3 diffuse = dot(-input.lightDir, worldNormal) * 0.5f + 0.5f;
	diffuse = diffuse * texColor;
	//diffuse = pow(diffuse, 4.0f); 조금 더 개선된 버전

	//스페큘러(정반사, Specular) 항
	float3 specular = 0;
	float3 viewDir = normalize(input.viewDir);
	float3 halfVector = (-lightDir) + (-viewDir);
	halfVector = normalize(halfVector);

	//Glossiness
	float Glossiness = 1.0f;


	if (diffuse.x > 0)
	{
		float3 HdotN = dot(halfVector, worldNormal);
		specular = pow(saturate(HdotN), Glossiness * Glossiness);
	}

	specular = specular * texColor;
	return float4(diffuse + specular, 1.0f);



	////텍스쳐 색상 샘플링(읽어오기)
	//float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	////Half 램버트 
	//float3 diffuse = input.diffuse * 0.5f + 0.5f; //디퓨즈 날려서 에러 발생
	//diffuse *= texColor.rgb; //색상값 추가

	////값 정리
	//float3 halfVector = normalize(input.halfVector);
	//float3 normal = normalize(input.normal);

	////스페큘러
	//float3 specular = 0;


	////빛의 강도가 0보다 클 때만 계산
	//if (diffuse.x > 0)
	//{
	//	float hDotn = dot(halfVector, normal);
	//	specular = saturate(hDotn);
	//	specular = pow(specular, 100.0f);
	//}

	//specular *= texColor.rgb;

	//return float4(diffuse + specular, 1.0f);
}