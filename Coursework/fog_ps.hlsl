// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer FogBuffer : register(b0)
{
	float4 bg;
	float fogS;
	float fogE;
	float fogStrength;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float fogFactor : FOG;
	float4 cameraPosition : CAM;
};


float4 main(InputType input) : SV_TARGET
{
	float4 fogColor;
float4 finalColor;


float4 textureColor;
textureColor = texture0.Sample(sampler0, input.tex);

fogColor = bg;
//calcuate fog colour based on forg factor and the colour inputted by the user
finalColor = (input.fogFactor * (0.0,0.0,0.0,0.0) + (1.0 - input.fogFactor) * fogColor);

//blend betwee the fog and the normal texture
float4 Fin = lerp(finalColor, (0.f,0.f,0.f,0.f), fogStrength);
return Fin;

}



