
struct DSInput{
	float4 Position : SV_Position;
	float4 Color : COLOR;
};
struct DSOutput{
	float4 Position : SV_Position;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

struct ConstantOutputType{
	float Edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};


[domain("tri")]
DSOutput DSEntry(ConstantOutputType input, float3 UVWCoord : SV_DomainLocation, const OutputPatch<DSInput,3> patch){
	DSOutput Output;
	Output.Position = UVWCoord.x * patch[0].Position + UVWCoord.y * patch[1].Position + UVWCoord.z * patch[2].Position;
	Output.Color = (patch[0].Color + patch[1].Color + patch[2].Color)/3.0f;
	Output.Color.w = 1.0f;
	Output.Normal = float3(0,0,0);
	
	return Output;
}
