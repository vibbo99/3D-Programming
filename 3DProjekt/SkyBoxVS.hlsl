struct vs_in
{
	float3 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};
struct vs_out
{
	float4 pos : SV_POSITION;
	float3 local_pos : POSITION;
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

	//output.pos = float4(input.normal, 1.0f);
	//matrix tempMatrix = mvp;

	//output.pos = mul(float4(input.pos, 1.0f),mvp).xyww;

	output.pos = mul(mvp, float4(input.pos, 1.0f)).xyww;

	/*output.pos = mul(model, float4(input.pos, 1.0f));
	output.pos = mul(view, output.pos);
	output.pos = mul(perspective, output.pos).xyww;*/

	//output.pos.z = 1.f;

	output.local_pos = input.pos;

	return output;
}