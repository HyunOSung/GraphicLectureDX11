// ��� ����.
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix viewProjection;
	float3 worldLightPosition;
	float3 worldCameraPosition;	
};

//�Է� ����ü
struct vs_input
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;

	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

//��� ����ü
struct vs_output
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;

	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;

	float3 normal : TEXCOORD3;
	float3 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
};

//���� �Լ�

vs_output main(vs_input input) //�ڵ� ��Ÿ �� Ȯ���ϱ�
{
	vs_output output;

	//���� ��ȯ
	output.position = mul(input.position, world);

	//����Ʈ ����
	output.lightDir = normalize(output.position.xyz - worldLightPosition);

	//���� ��� , ���� ��� ���� �Է��� ��� ���� ������ ������ ��ֶ������ؾ� ��
	output.normal = normalize(mul(input.normal, (float3x3)world));

	//���� ź��Ʈ
	output.tangent = normalize(mul(input.tangent, (float3x3)world));

	//���� ���̳��
	output.binormal = normalize(mul(input.binormal, (float3x3)world));

	//�� ����
	output.viewDir = normalize(output.position.xyz - worldCameraPosition);

	////Half ����(�� �� ���̴��� �ٽ�)
	//output.halfVector = normalize((-lightDir) + (-viewDir));

	//��/���� ��ȯ
	output.position = mul(output.position, viewProjection);
	
	//UV ��ǥ ����
	output.texCoord = input.texCoord;

	////���ݻ籤(��ǻ��, diffuse)
	//output.diffuse = dot(-lightDir, output.normal);

	//��ȯ
	return output;
}
