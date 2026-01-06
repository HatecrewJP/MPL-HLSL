RWTexture2D <unorm float4> SwapChainImageCopy : register(u0);
RWTexture2D <uint4> CubeImage : register(u1);


[numthreads(1,1,1)]
void CSEntry(uint3 GroupID: SV_GroupID){
	uint2 Coordinate = uint2(GroupID.x,GroupID.y)%64;
	
	//float4 Color = SwapChainImageCopy[Coordinate];
	//Color.r *= 0.7f;
	float4 Color = normalize((float4)CubeImage[Coordinate]);
	
	SwapChainImageCopy[Coordinate] = Color;
}