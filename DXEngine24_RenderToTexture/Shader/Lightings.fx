float3 OrenNayer(
	float4 position, 
	float3 lightPosition, 
	float3 viewPosition,
	float roughness,
	float3 normal)
{
	//����Ʈ ����
    float3 lightDir = normalize(position.xyz - lightPosition);

	//�交��
    float3 viewDir = normalize(position.xyz - viewPosition);

	//�����Ͻ�
    //float roughness = 0.6f;
    float PI = 1.0f;

	//��ǻ�� ���
    float roughness2 = roughness * roughness;
    normal = normalize(normal);

    float A = 1.0f - 0.5f * (roughness2 / (roughness2 + 0.33));
    float B = 0.45f * (roughness2 / (roughness2 + 0.09f));
	
	//LdotN / VdotN.
    float LdotN = dot(-lightDir, normal);
    float VdotN = dot(-viewDir, normal);

	//����(X-Y �������, Tangent-Binormal ���)
    float3 lightProjection = normalize(lightDir - normal * LdotN);
    float3 viewProjection = normalize(viewDir - normal * VdotN);

	//����Ʈ ���� ���Ϳ� �� �������� ������ �ڻ��� ���� ���
    float C = saturate(dot(lightProjection, viewProjection));
	//float C = max(0, dot(lightProjection, viewProjection));

	//theta_i
    float incidentAngle = acos(LdotN);

	//theta_r
    float viewAngle = acos(VdotN);

	//Alpha / Beta ���ϱ�
    float alpha = max(incidentAngle, viewAngle);
    float beta = min(incidentAngle, viewAngle);
		
    float D = sin(alpha) * tan(beta);
	
	//0���� �ڸ���
    LdotN = saturate(LdotN);
    float albedo = 1.0f;
    float coe = albedo / PI;

    
    //float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
    float3 ON_Diffuse = (1 / PI) * LdotN * (A + B * C * D);
   
    return ON_Diffuse;
}