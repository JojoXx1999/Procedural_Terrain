// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float blend;
	float3 padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};



float4 main(InputType input) : SV_TARGET
{
	//float4 lightColor;
	float4 textureColour;
textureColour = texture0.Sample(sampler0, input.tex);

//get the average of the rgb values from the inputted texture to calculate grey scale
float avg = (textureColour.x + textureColour.y + textureColour.z) / 3;
float4 greyColour = float4(avg, avg, avg, 1);


/*
----------------------------bloom
float4 final = textureColour - float4(1.f, 1.f, 1.f, 0.f);
if (final.x < 0) final.x = 0;
if (final.y < 0) final.y = 0;
if (final.z < 0) final.z = 0;
*/

float4 fin = lerp(textureColour, greyColour, blend);

return fin;
}



