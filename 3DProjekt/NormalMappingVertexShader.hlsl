struct vs_in
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};
struct vs_out
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 normal : NORMAL;
	float4 world_pos : WORLD_POS;
	float4 tangent : TANGENT;
	//float4x4 TBN : TBNMATRIX;
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
	vs_out output = (vs_out)0;

	output.pos = mul(mvp, float4(input.pos, 1.0f));
	output.world_pos = mul(model, float4(input.pos, 1.0f));
	//output.normal = mul(model, float4(input.normal, 1.0f));
	output.normal = float4(input.normal, 1.0f);
	output.texcoord = input.texcoord;

	//output.normal = mul(model, float4(input.bitangent, 1.0f));

	//float4 T = normalize(float4(mul(model, float4(input.tangent, 0.0))));
	//float4 B = normalize(float4(mul(model, float4(input.bitangent, 0.0))));
	//float4 N = normalize(float4(mul(model, float4(input.normal, 0.0))));
	//
	//float4x4 TBN = float4x4(T, B, N, float4(0.0, 0.0, 0.0, 0.0));
	//output.TBN = TBN;
	
	output.tangent = mul(model, float4(input.tangent, 1.0f));
	return output;
}