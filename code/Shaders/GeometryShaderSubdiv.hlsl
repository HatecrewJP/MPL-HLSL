struct GSOutput{
	float4 Pos : SV_Position;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

struct GSInput{
	float4 Position : SV_Position;
	float4 Color : COLOR;
};

float3 CalculateNormalFromTriangle(float4 In1, float4 In2, float4 In3){
	
	float4 PlaneVec1 = In2 - In1;
	float4 PlaneVec2 = In3 - In2;
	return normalize(cross(PlaneVec1.xyz,PlaneVec2.xyz));
}

[maxvertexcount(9)]
void GSEntry(triangle GSInput InputTri[3] : SV_Position, inout TriangleStream<GSOutput> OutStream){
	float4 MidPoint = (InputTri[0].Position + InputTri[1].Position + InputTri[2].Position)/3;
	MidPoint.w = 1.0f;
	float4 MidPointColor = float4((InputTri[0].Color.xyz + InputTri[1].Color.xyz + InputTri[2].Color.xyz)/3,1);
	GSOutput Output;
	Output.Color =  MidPointColor;
	Output.Color.w = 1.0f;
	
	Output.Normal = CalculateNormalFromTriangle(InputTri[0].Position,InputTri[1].Position,MidPoint);
	Output.Color.xyz = (InputTri[0].Color.xyz + InputTri[1].Color.xyz + MidPointColor.xyz)/3.0f;
	Output.Pos = InputTri[0].Position;
	OutStream.Append(Output);
	Output.Pos = InputTri[1].Position;
	OutStream.Append(Output);
	Output.Pos = MidPoint;
	OutStream.Append(Output);
	
	Output.Normal = CalculateNormalFromTriangle(InputTri[1].Position,InputTri[2].Position,MidPoint);
	Output.Color.xyz = (InputTri[1].Color.xyz + InputTri[2].Color.xyz + MidPointColor.xyz)/3.0f;
	Output.Pos = InputTri[2].Position;
	OutStream.Append(Output);
	
	Output.Normal = CalculateNormalFromTriangle(InputTri[2].Position,InputTri[0].Position,MidPoint);
	Output.Color.xyz = (InputTri[2].Color.xyz + InputTri[0].Color.xyz + MidPointColor.xyz)/3.0f;
	Output.Pos = InputTri[0].Position;
	OutStream.Append(Output);
	OutStream.RestartStrip();
	
	
}