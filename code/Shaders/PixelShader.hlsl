#define DEBUGZ 0
#define IMPRECISIONFIX 1
cbuffer CBuffer : register(b0){
	float RotationAngle : packoffset(c0.x);
	float Width  : packoffset(c0.y);
	float Height : packoffset(c0.z);
	float ColorR : packoffset(c1.x);
	float ColorG : packoffset(c1.y);
	float ColorB : packoffset(c2.x);
	float ColorA : packoffset(c2.y);
};

struct ps_input{
	float4 vPosition : SV_Position;
 	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

struct ps_output{
	float4 Color : SV_Target0;
};

ps_output PSEntry(const ps_input input){
	ps_output output;
	output.Color = input.Color;
	
#if IMPRECISIONFIX
	[branch]if(input.Color.r >= 0.99999995f){
		output.Color = float4(ColorR,ColorG,ColorB,ColorA);
	}
#else
	[branch]if(input.Color.r == 1.0f){
		output.Color = float4(ColorR,ColorG,ColorB,ColorA);
	}
#endif
	
	[flatten]if(input.Color.r == 0.0f){
		output.Color = float4(ColorR,ColorG,ColorB,ColorA);
	}
	
	return output;
}