Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
	float3 rgb;
	float padding;;
	float3 frequency;
	float time;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	//set up variables
	float4 colour;
//offset the colour channels
//multiply by frequency to create the wave effect in each channel
//time is used to move the image with the frequency
float R = texture0.Sample(sampler0, float2((input.tex.x - time) * frequency.x, 0.5)).r;
float G = texture0.Sample(sampler0, float2((input.tex.x - time) * frequency.y, 0.5)).g;
float B = texture0.Sample(sampler0, float2((input.tex.x - time) * frequency.z, 0.5)).b;
//Offset the position of these channels
float3 CA = rgb * float3(R, G, B);


//Create variables for the returned 
//set variables to the input texture plus the offset channels
float4 ColourR = texture0.Sample(sampler0,	float2(input.tex.x + CA.x , input.tex[1]),0);
float4 ColourG = texture0.Sample(sampler0, float2(input.tex.x + CA.y, input.tex[1]),0);
float4 ColourB = texture0.Sample(sampler0, float2(input.tex.x + CA.z, input.tex[1]),0);

//return the chromatic aberrated scene
colour = float4(ColourR.r , ColourG.g, ColourB.b, 1.0f);
return colour;
}