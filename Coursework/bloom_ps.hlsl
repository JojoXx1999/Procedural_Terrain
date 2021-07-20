// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};



float4 main(InputType input) : SV_TARGET
{
	float4 textureColour, blurTexture;
//
textureColour = texture0.Sample(sampler0, input.tex);
//blur texture from render to texture
blurTexture = texture1.Sample(sampler0, input.tex);

//Get brightest parts of image, clamp minimum to 0 to avoid strange colouration
float4 final = textureColour - float4(1.f, 1.f, 1.f, 0.f);
if (final.x < 0) final.x = 0;
if (final.y < 0) final.y = 0;
if (final.z < 0) final.z = 0;
final = final * blurTexture;
final = final + textureColour;
return final;
}