
struct ps_input
{
	float4 vPosition : SV_Position;
};

struct ps_output
{
	float4 color : SV_Target0;
};

ps_output PSEntry(const ps_input input)
{
	ps_output output;
	
	output.color = float4(1,1,1,1);

	

	
	return output;
}