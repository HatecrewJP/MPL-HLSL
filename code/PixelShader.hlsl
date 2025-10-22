
struct ps_input
{
	float4 vPosition : POSITION;
};

struct ps_output
{
	float4 color : COLOR;
};

ps_output PSEntry(const ps_input input): COLOR
{
	ps_output output;
	output.color = 1;
	
	float4 one = {1,1,1,1};
	
	switch((input.vPosition.x%3))
	{
		case 0:{
			output.color.x = one.x;	
		}break;
		
		case 2:{
			output.color.y = one.y;
		}break;
		
		case 3:{
			output.color.z = one.z;
		}break;
	}
	switch((input.vPosition.y%3))
	{
		case 0:{
			output.color.x = one.x;	
		}break;
		
		case 2:{
			output.color.y = one.y;
		}break;
		
		case 3:{
			output.color.z = one.z;
		}break;
	}
	switch((input.vPosition.y%3))
	{
		case 0:{
			output.color.x = one.x;	
		}break;
		
		case 2:{
			output.color.y = one.y;
		}break;
		
		case 3:{
			output.color.z = one.z;
		}break;
	}
	output.color.w += one.w;
	return output;
}