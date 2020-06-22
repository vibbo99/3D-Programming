struct vs_in
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};
struct vs_out
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};
struct skymap_vs_out 
{
	float4 pos : SV_POSITION;
	float3 texcoord : TEXCOORD;
};

cbuffer constantBuffer : register (b0)
{
	matrix model;
	matrix view;
	matrix perspective;
	matrix mvp;
}

vs_out vs_main(vs_in input)
{
	output.pos = mul(inPos, mvp);

	ouput.normal = mul(normal, model);

	output.TexCoord = inTexCoord;

	return output;
}

skymap_vs_out skymap_vs(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	vs_out output = (vs_out)0;

	output.pos = mul(float4(input.pos, 1.0f), mvp).xyww;

	output.texcoord = input.texcoord;

	return output;
}

