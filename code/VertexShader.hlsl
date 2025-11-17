
struct vs_input
{
	float3 vPosition : SV_Position;
};

struct vs_output
{
	float4 vPosition : SV_Position;
};


vs_output VSEntry(const vs_input input,uniform float3 offset)
{
	vs_output output;
	float3 VertexOffest = offset;
	output.vPosition = float4(input.vPosition - VertexOffest,1);
	output.vPosition.xyz = output.vPosition.xyz / 1.5;
	
	return output;
}