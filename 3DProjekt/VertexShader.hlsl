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
	float4 normal : NORMAL;
	float4 world_pos : WORLD_POS;
	float4 cam_pos : CAMERA_POS;
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

	//output.pos = float4(input.pos, 1.0f);
	//output.normal = input.normal;
	//output.color = input.color;

	//float4 newPos = 
	output.pos = mul(mvp, float4(input.pos, 1.0f));
	output.texcoord = input.texcoord;

	output.normal = float4(input.normal, 1.0f); 
	//output.normal = mul(model, float4(input.normal, 1.0f));
	output.world_pos = mul(model, float4(input.pos, 1.0f));


	return output;
}