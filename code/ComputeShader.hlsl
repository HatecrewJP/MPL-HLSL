RWTexture2D <unorm float4> SwapChainImageCopy : register(u0);

[numthreads(1,1,1)]
void CSEntry(uint3 GroupID: SV_GroupID){
	uint2 Coordinate = uint2(GroupID.x,GroupID.y);
	
	float4 Color = SwapChainImageCopy[Coordinate];
	bool ColorIsWhite = (Color.x+Color.y+Color.z+Color.w) == 4;
	
	[flatten]if(!ColorIsWhite){
		Color.r *= 0.7f;
	}
	SwapChainImageCopy[Coordinate] = Color;
}