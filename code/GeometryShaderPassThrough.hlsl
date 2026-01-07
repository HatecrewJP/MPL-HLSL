struct GSOutput{
	float4 Pos : SV_Position;
	float4 Color : COLOR;
};

struct GSInput{
	float4 Pos: SV_Position;
	float4 Color : COLOR;

};


[maxvertexcount(9)]
void GSEntry(triangle GSInput InputTri[3] : SV_Position, inout TriangleStream<GSOutput> OutStream){	
	GSOutput output;
	output.Color.xyz = (InputTri[0].Color.xyz);
	output.Color.w = 1.0f;
	output.Pos = InputTri[0].Pos;
	OutStream.Append(output);
	output.Color.xyz = (InputTri[1].Color.xyz);
	output.Pos = InputTri[1].Pos;
	OutStream.Append(output);
	output.Pos = InputTri[2].Pos;
	output.Color.xyz = (InputTri[2].Color.xyz);
	OutStream.Append(output);
}