//depth buffer pixel shader
struct InputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	float depthValue;
// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
depthValue = input.depthPosition.z / input.depthPosition.w;

//return the depth values to be used by the shadow shader
return float4(depthValue, depthValue, depthValue, 1.0f);

}