// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer FogBuffer : register(b1)
{
	float fogStart;
	float fogEnd;
}

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float fogFactor : FOG;
	float4 cameraPosition : CAM;
};

OutputType main(InputType input)
{
	OutputType output;

	output.position = mul(input.position, worldMatrix);
	output.worldPosition = output.position;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	//ouput.color = input.inColor;

	//get the position of the camer
	float4 cam = output.cameraPosition = mul(input.position, worldMatrix);
	output.cameraPosition = mul(cam, viewMatrix);

	//calculate a fog factor based on the camera position, so that the fog dissapears the closer the user gets to the terrain
	output.fogFactor = (saturate((fogEnd - output.cameraPosition.z) / (fogEnd - fogStart)));

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}