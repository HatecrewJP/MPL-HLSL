struct PatchControlPoint{
	float4 Position : SV_Position;
	float4 Color : COLOR;
};

struct DSOutput{
	float4 Position : SV_Position;
	float4 Color : COLOR;
};

struct ConstantOutputType{
	float Edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};


[domain("tri")]
DSOutput DSEntry(ConstantOutputType input, float3 UVWCoord : SV_DomainLocation, const OutputPatch<PatchControlPoint,3> patch){
	DSOutput Output;
	Output.Position = UVWCoord.x * patch[0].Position + UVWCoord.y * patch[1].Position + UVWCoord.z * patch[2].Position;
	Output.Color = UVWCoord.x * patch[0].Color+ UVWCoord.y * patch[1].Color + UVWCoord.z * patch[2].Color;
	Output.Color.w = 1.0f;
	return Output;
}
