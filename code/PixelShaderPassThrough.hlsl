#define DEBUGZ 0
struct ps_input
{
	float4 vPosition : SV_Position;
 	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

struct ps_output
{
	float4 color : SV_Target0;
};

ps_output PSEntry(const ps_input input)
{
	ps_output output;
	output.color = input.Color;
#if DEBUGZ
	if(input.vPosition.z > 0){
		output.color = float4(1,0,0,1);
	}
#endif
	return output;
}