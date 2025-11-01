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
	output.vPosition.xyzw = float3(input.vPosition.xyz,1);
	
	if(input.vPosition.x < 0){
		output.vPosition.x = 0;
	}
	
	
	return output;
}