struct ps_in
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 normal : NORMAL;
	float4 world_pos : WORLD_POS;
};

float4 ps_main(ps_in input) : SV_TARGET
{

	return float4(1.0f, 1.0f, 1.0f, 1.0f);

}