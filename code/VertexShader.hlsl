
struct vs_input
{
	uint3 vPosition : POSITION;
};

struct vs_output
{
	float4 vPosition;
};

vs_output VSEntry(const vs_input input):POSITION
{
	vs_output output;
	float4 x = {1.0f,100.0f,0.0f,0.0f};
	output.vPosition = x;
	return output;
}