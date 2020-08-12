struct vs_in
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};
struct vs_out
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};


vs_out vs_main(vs_in input)
{
	vs_out output = (vs_out)0;


	output.pos =  float4(input.pos, 1.0f);
	output.texcoord = input.texcoord;



	return output;
}