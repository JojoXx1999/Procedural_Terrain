// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

Texture2D reflectionTexture : register(t1);
SamplerState sampler1 : register(s1);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColor;
float2 reflectTexCoord;
float4 reflectionColor;
float4 color;

// Sample the texture pixel at this location.
textureColor = texture0.Sample(sampler0, input.tex);

// Calculate the projected reflection texture coordinates.
reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

// Sample the texture pixel from the reflection texture using the projected texture coordinates.
reflectionColor = reflectionTexture.Sample(sampler0, input.tex);

// Do a linear interpolation between the two textures for a blend effect.
color = lerp(textureColor, reflectionColor, 0.15f);

return color;
}



