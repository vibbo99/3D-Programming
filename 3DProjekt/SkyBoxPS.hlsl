struct ps_in
{
	float4 pos : SV_POSITION;
	float3 local_pos : POSITION;
};

TextureCube myTexture: register(t0);
SamplerState samplerState;

float4 ps_main(ps_in input) : SV_TARGET
{
	//Value of alpha is set to 0.5 to be able to detect it in second pass (glowPass)
	return float4(myTexture.Sample(samplerState, input.local_pos).xyz, 0.f);
	
}