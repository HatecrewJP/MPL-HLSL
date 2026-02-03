struct GSOutput{
	float4 Pos : SV_Position;
	float4 Color : COLOR;
};

struct GSInput{
	float4 Position : SV_Position;
	float4 Color : COLOR;
};


[maxvertexcount(9)]
void GSEntry(triangle GSInput InputTri[3] : SV_Position, inout TriangleStream<GSOutput> OutStream){
	float4 MidPoint = (InputTri[0].Position + InputTri[1].Position + InputTri[2].Position)/3.0f;
	float4 MidPointColor = float4(InputTri[0].Color + InputTri[1].Color + InputTri[2].Color)/3.0f;
	
	GSOutput Output;
	Output.Pos = InputTri[0].Position;
	Output.Color = InputTri[0].Color;
	OutStream.Append(Output);
	Output.Pos = InputTri[1].Position;
	Output.Color = InputTri[1].Color;
	OutStream.Append(Output);
	Output.Pos = MidPoint;
	Output.Color = MidPointColor;
	OutStream.Append(Output);
	
	Output.Pos = InputTri[2].Position;
	Output.Color = InputTri[2].Color;
	OutStream.Append(Output);
	
	Output.Pos = InputTri[0].Position;
	Output.Color = InputTri[0].Color;
	OutStream.Append(Output);
	OutStream.RestartStrip();
}