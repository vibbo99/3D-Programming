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
};

vs_out main(vs_in input)
{
    vs_out output;
    output.pos = float4(input.pos, 1.f);
    output.texcoord = input.texcoord;
    output.normal = float4(input.normal, 0.f);

    return output;
}

