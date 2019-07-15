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

//���� �Լ�
float4 main(ps_input input) : SV_TARGET
{
	//��ȯ ���
	float3x3 TBN = float3x3(
		normalize(input.tangent),
		normalize(input.binormal),
		normalize(input.normal)
	);

	float3 tangentNormal = normalMap.Sample(diffuseSampler, input.texCoord).rgb;
	tangentNormal = tangentNormal * 2 - 1;

	//���� ��ȯ
	float3 worldNormal = mul(tangentNormal, transpose(TBN));
	worldNormal = normalize(worldNormal);

	//���ݻ�(��ǻ��, Diffuse) ��
	float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
	float3 lightDir = normalize(input.lightDir);
	float3 diffuse = dot(-input.lightDir, worldNormal) * 0.5f + 0.5f;
	diffuse = diffuse * texColor;
	//diffuse = pow(diffuse, 4.0f); ���� �� ������ ����

	//����ŧ��(���ݻ�, Specular) ��
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



	////�ؽ��� ���� ���ø�(�о����)
	//float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	////Half ����Ʈ 
	//float3 diffuse = input.diffuse * 0.5f + 0.5f; //��ǻ�� ������ ���� �߻�
	//diffuse *= texColor.rgb; //���� �߰�

	////�� ����
	//float3 halfVector = normalize(input.halfVector);
	//float3 normal = normalize(input.normal);

	////����ŧ��
	//float3 specular = 0;


	////���� ������ 0���� Ŭ ���� ���
	//if (diffuse.x > 0)
	//{
	//	float hDotn = dot(halfVector, normal);
	//	specular = saturate(hDotn);
	//	specular = pow(specular, 100.0f);
	//}

	//specular *= texColor.rgb;

	//return float4(diffuse + specular, 1.0f);
}