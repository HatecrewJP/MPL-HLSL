RWTexture2D <unorm float4> SwapChainImageCopy : register(u0);

[numthreads(1,1,1)]
void CSEntry(uint3 GroupID: SV_GroupID){
	uint2 Coordinate = uint2(GroupID.x,GroupID.y);
	
	float4 Color = SwapChainImageCopy[Coordinate];
	Color.r *= 0.7f;
	SwapChainImageCopy[Coordinate] = Color;
}