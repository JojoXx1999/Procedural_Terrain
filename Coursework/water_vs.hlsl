cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer FogBuffer : register(b2)
{
	float fogStart;
	float fogEnd;
}

cbuffer ReflectionBuffer
{
	matrix reflectionMatrix;
};

cbuffer timeBuffer : register(b1)
{
	//variables to change the vertex manipulation
	float time;
	float amplitude;
	float frequency;
	float speed;
	float3 padding;
	float height;
	float3 camPos;
	float padding2;

};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	uint id : SV_VertexID;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float fogFactor : FOG;
	float3 cameraPosition : CAM;
	float3 viewVector: TEXCOORD1;
};

float3 getHeight(float x, float z)
{
	//change vertices using a sin and cos wave and the values inputted by the user
	float wave = (amplitude * sin(frequency * (x + (time * speed)))) + (amplitude * cos(frequency * (x + (time * speed / 4))));
	return wave;
};

float3 updateNormals(float3 Pos, float3 normal) {
	//re-calculate normals using the surroudning pixels
	//offset to surrounding pixel
	float offset = 1;
	//use the current pixel and offset to get the neibouring pixel
	float3 P1 = Pos.xyz;
	P1.x = Pos.x - offset;	P1.z = Pos.z; P1.y = getHeight(P1.x, P1.z);

	//use the current pixel and offset to get the neibouring pixel
	float3 P2 = Pos.xyz;
	P2.x = Pos.x + offset;	P2.z = Pos.z;	P2.y = getHeight(P2.x, P2.z);

	//use the current pixel and offset to get the neibouring pixel
	float3 P3 = Pos.xyz;
	P3.x = Pos.x; P3.z = Pos.z - offset; P3.y = getHeight(P3.x, P3.z);

	//use the current pixel and offset to get the neibouring pixel
	float3 P4 = Pos.xyz; P4.x = Pos.x; P4.z = Pos.z + offset; P4.y = getHeight(P4.x, P4.z);

	//create 2 vectors using the surronding pixels
	float3 V1 = normalize(P2 - P1);
	float3 V2 = normalize(P4 - P3);

	//use the cross product of these vectors to work out the normal of the current pixel
	normal = cross(V2, V1);
	return normal;
}

OutputType main(InputType input)
{
	OutputType output;

	matrix reflectProjectWorld;

	input.position.y = getHeight(input.position.x, input.position.z) + height;
	input.normal = updateNormals(input.position, input.normal);

	float4 worldPos = mul(input.position, worldMatrix);
	output.viewVector = camPos.xyz - worldPos.xyz;
	output.viewVector = normalize(output.viewVector);


	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	float3 cam = output.cameraPosition = mul(input.position, worldMatrix);
	cam = output.cameraPosition = mul(cam, viewMatrix);

	output.fogFactor = (saturate((fogEnd - cam.z) / (fogEnd - fogStart)));

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}