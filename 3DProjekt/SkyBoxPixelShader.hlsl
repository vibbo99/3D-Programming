struct ps_in
{
	float4 pos : SV_POSITION;
	float3 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

Texture2D myTexture: register(t0);
SamplerState samplerState : register(s0);
TextureCube SkyMap;

float4 ps_main(vs_output input) : SV_TARGET
{
	float4 diffuse = myTexture.Sample(samplerState, input.texcoord);

	return diffuse;
}

float4 skymap_ps(skymap_vs_out input) : SV_TARGET
{
	return SkyMap.Sample(samplerState, input.texcoord);

}

float4 D2D_PS(vs_output input) : SV_TARGET
{
	float4 diffuse = myTexture.Sample(samplerState, input.texcoord);

	return diffuse;
}