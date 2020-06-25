struct gs_in
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 normal : NORMAL;
    float4 world_pos : WORLD_POS;
    float4 cam_pos : CAMERA_POS;
};


struct gs_out
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 normal : NORMAL;
    float4 world_pos : WORLD_POS;
    float4 cam_pos : CAMERA_POS;
};

cbuffer constantMatrixBuffer : register(b0)
{
    matrix world;
    matrix worldView;
    matrix worldViewProjection;
};

[maxvertexcount(3)]
void main(
	triangle gs_in input[3] : SV_POSITION,
	inout TriangleStream<gs_out> output)
{
    float3 v1 = mul(worldView, input[1].pos).xyz - mul(worldView, input[0].pos).xyz;
    float3 v2 = mul(worldView, input[2].pos).xyz - mul(worldView, input[0].pos).xyz;
    float3 triNormal = normalize(cross(v1, v2));

    float3 centerVector = float3(0.f, 0.f, 0.f) - mul(worldView, input[0].pos).xyz;
    
    if (dot(triNormal, centerVector) > 0.f)
    {
        for (uint i = 0; i < 3; i++)
        {
            gs_out element;
            element.pos = mul(worldViewProjection, input[i].pos);
            element.texcoord = input[i].texcoord;
            element.normal = normalize(mul(world, input[i].normal));
            element.world_pos = mul(world, input[i].pos);
            output.Append(element);
        }
    }
}