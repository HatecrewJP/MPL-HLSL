
struct vs_input
{
	float3 vPosition : POSITION;
};

struct vs_output
{
	float4 vPosition;
};

vs_output VSEntry(const vs_input input):POSITION
{
	vs_output output;
	float3 VertexOffest = float3(0,0.25,0);
	output.vPosition = float4(input.vPosition - VertexOffest,1);
	output.vPosition.xyz = output.vPosition.xyz / 2;


	return output;
}