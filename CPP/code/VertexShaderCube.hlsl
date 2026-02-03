#define PI 3.14159265359
#define DegreeToRad(x) ((x)*PI/180)

uniform float RotationAngle : register(b0[0]);
uniform float Width : register(b0[1]);
uniform float Height: register(b0[2]);
	
struct vs_input
{
	float3 vPosition : SV_Position;
	float4 Color : COLOR;
};

struct vs_output
{
	float4 vPosition :SV_Position;
	float4 Color : COLOR;
};

float4 RotationYaw(float4 Vec4,float Angle){
	float RadAngle = DegreeToRad(Angle);
	float4x4 YawRotationMatrix = {
		cos(RadAngle),-sin(RadAngle),0.0f,0.0f,
		sin(RadAngle),cos(RadAngle),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f};
		
	return mul(YawRotationMatrix,Vec4);
}
float4 RotationPitch(float4 Vec4,float Angle){
	float RadAngle = DegreeToRad(Angle);
	float4x4 PitchRotationMatrix = {
		cos(RadAngle),0.0f,-sin(RadAngle),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin(RadAngle),0.0f,cos(RadAngle),0.0f,
		0.0f,0.0f,0.0f,1.0f};
		
	return mul(PitchRotationMatrix,Vec4);
}
float4 RotationRoll(float4 Vec4,float Angle){
	float RadAngle = DegreeToRad(Angle);
	float4x4 RollRotationMatrix = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos(RadAngle),-sin(RadAngle),0.0f,
		0.0f,sin(RadAngle),cos(RadAngle),0.0f,
		0.0f,0.0f,0.0f,1.0f};
		
	return mul(RollRotationMatrix,Vec4);
}


vs_output VSEntry(const vs_input input){
	vs_output Output;
	float4 Input = float4(input.vPosition,1);
	
	float4 ScalingVec = {1.1f,1.1f,1.1f,1};
	float4x4 OrthographicProjectionMatrix = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,(2.0f/3.0f),3.0f/2.0f,
		0.0f,0.0f,0.0f,1.0f};
	Input = Input * ScalingVec;
	Input = RotationYaw(Input,RotationAngle);
	Input = RotationPitch(Input,RotationAngle);
	Input = RotationRoll(Input,0);
	Input.x /= (Width/Height);
	Input = mul(OrthographicProjectionMatrix,Input);
	
	Output.vPosition = Input;
	Output.Color =  input.Color;
	return Output;
}

