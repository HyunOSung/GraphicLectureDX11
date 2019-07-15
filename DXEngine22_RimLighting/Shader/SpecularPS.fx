//�ȼ� ���̴� �Է�
// ���̴�/���̴�(shader)


struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

// �ؽ�ó / ���÷� ������Ʈ.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�.
float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó ���� �о����.
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// ����ó�� (��ǻ��).

	// = float3 diffuse = max(0, inputSettings)
	float3 diffuse = saturate(input.diffuse);
	diffuse = diffuse * textureColor.rgb; //�ؽ��� �ε��ϴ� �κ�

	// �� ����
	float3 reflection = normalize(input.reflection);
	float3 viewDir = normalize(input.viewDir);

	//����ŧ��
	float3 specular = 0;

	//���� ������ 0���� ū ��츸 ���
	if (diffuse.x > 0)
	{
		float3 rDotv = dot(reflection, -viewDir);
		specular = saturate(rDotv);
		specular = pow(specular, 20.0f); //����ŧ���� ����, ���� ����
	}

	float3 ambient = { 0.05f, 0.05f, 0.05f };

	float3 specularColor = {1.0f, 0.0f, 0.0f };
	//float3 finalColor = (1.0f - specular) * specularColor;
	//�������� �����ϸ� ���� �� �ȳ���.
	//���ڷ� �����Ѵٸ� 0�̻� �Ǵ� ���� ���� ����ŧ�� ���̴� ���� ���������� �ƽ����� 1���� ���ָ� ������ ���� ���´�.

	return float4(diffuse + specular , 2.0f);
}