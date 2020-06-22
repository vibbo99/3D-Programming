struct ps_in
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 normal : NORMAL;
	float4 world_pos : WORLD_POS;
	float4 tangent : TANGENT;
	//float4x4 TBN : TBNMATRIX;
};
Texture2D myTexture: register(t0);
Texture2D normalTexture : register(t1);
SamplerState samplerState;
struct Point_Light
{
	float4 l_Position;
	float4 l_Ambient;
	float4 l_Diffuse;
	float4 l_Specular;
	float4 l_Attenuation;
};
cbuffer CB_LIGHT : register(b0)
{
	Point_Light pointLight[6];
}

cbuffer Nr_Lights : register(b1)
{
	//x = point_Lights
	//y = directional_Lights
	//z = spot_Lights
	float4 light_Nrs;
}
cbuffer Camera : register(b2)
{
	float4 camera_pos;
}

float4 calcPointLight(Point_Light pl, ps_in input);
float4 ps_main(ps_in input) : SV_TARGET
{

	float4 result = (0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < light_Nrs.x; i++)
	{
		result += calcPointLight(pointLight[i], input);
	}
	return result;

}
float4 calcPointLight(Point_Light pl, ps_in input)
{
	float3 ambient = pl.l_Ambient;
	float4 tex = myTexture.Sample(samplerState, input.texcoord);
	ambient *= tex.xyz;

	//Diffuse
	//float3 norm = normalize(input.normal);
	//float4 norm = normalTexture.Sample(samplerState, input.texcoord);
	//norm = normalize(norm * 2.0 - 1.0);
	//norm = normalize(mul(input.TBN, norm));
	float4 normalMap = normalTexture.Sample(samplerState, input.texcoord);
	normalMap = (2.0f * normalMap) - 1.0f;
	input.tangent = normalize(input.tangent -  dot(input.tangent, input.normal) * input.normal);

	float3 biTangent = cross(input.normal, input.tangent);
	float3x3 texSpace = float3x3(input.tangent.xyz, biTangent, input.normal.xyz);
	float3 norm = normalize(mul(normalMap, texSpace));
	
	float3 lightDir = normalize(pl.l_Position - input.world_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	float3 diffuse = pl.l_Diffuse * diff * tex.xyz;

	float distance = length(pl.l_Position - input.world_pos);
	float attenuation = 1.0 / (pl.l_Attenuation.x +
		pl.l_Attenuation.y * distance + pl.l_Attenuation.z * (distance * distance));
	//Specular

	float specularStr = 0.5;
	float3 viewDir = normalize(camera_pos - input.world_pos);
	float3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	float3 specular = specularStr * spec * pl.l_Diffuse;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float3 result = ambient + diffuse + specular * tex.xyz;
	return float4(result, 1.0f);
	//return float4(norm, 1.0f);
}
