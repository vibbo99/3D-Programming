// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
	float4 pos          : SV_POSITION;
	float2 texcoord		: TEXCOORD; // Denna slutar med en nolla 0
	float4 normal       : NORMAL;
	float4 world_pos    : WORLD_POS;
	//float3 tanget		: TANGENT; // Normals=?

	// TODO: change/add other stuff
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : BEZIERPOS;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]		: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor		: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain

	float3 tangent[3]			: TANGENT;
	float3 texcoord[3]			: TEXCOORD0; // Denna slutar med en nolla 0
	float3 tanUCorner[3]		: TANUCORNER;
	float3 tanVCorner[3]		: TANVCORNER;
	float3 CWts                 : TANWEIGHTS;

	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = 
	Output.EdgeTessFactor[1] = 
	Output.EdgeTessFactor[2] = 
	Output.InsideTessFactor = 15; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")] 
[partitioning("fractional_odd")] // "pow2" might look better
[outputtopology("triangle_cw")]  // ccw for counter clockwise
[outputcontrolpoints(3)]         // "number of times the main function will be executed"
[patchconstantfunc("CalcHSPatchConstants")] // func name
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.pos = ip[i].pos;

	return Output;
}
