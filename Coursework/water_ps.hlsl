
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState sampler0 : register(s0);

cbuffer WaterBuffer : register(b0)
{
	//variables for lighting
	float4 diffuseColour;
	float3 lightDirection;
	float alpha;
	float3 padding;
	float fogStrength;
	float4 bg;
	float directional;
	float spot;
	float pointL;
	float time;
	float change;
	float3 padding2;
	//variables for specular highlight
	float constFac;
	float linFac;
	float dist;
	float quad;
	float specularPower;
	float3 padding3;
	float4 specularColour;

};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float fogFactor : FOG;
	float3 cameraPosition : CAM;
	float3 viewVector: TEXCOORD1;
};

float4 calcSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float4 specularPower)
{// blinn-phongspecular calculation
	float3 halfway = normalize(lightDirection + viewVector);
	float specularIntensity = pow(max(dot(normal, halfway), 0.0), specularPower);
	return saturate(specularColour * specularIntensity);
}

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 Texture(float3 normal, float fogFactor, float2 tex) {
	float4 textureColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, tex);

	float4 Color2 = lerp(textureColour, (0.f, 0.f, 0.f, 0.f), 0);

	//textureColour = lerp((0.f,0.f,0.f,0.f), textureColour, alpha);

	float4 fogColor;
	float4 finalColor;

	fogColor = bg;
	finalColor = (fogFactor * Color2 + (1.0 - fogFactor) * fogColor);

	float4 Fin = lerp(Color2, finalColor, fogStrength);

	textureColour = lerp((0.f, 0.f, 0.f, 0.f), Fin, alpha);
	Fin = lerp(textureColour, finalColor, fogStrength);
	return Fin;
}

float4 changeColour(float4 Fin) {
	//cycle though colour using a cos wave
	float4 varColour;
	float4 red = float4(255, 0, 0, 1);
	float4 green = float4(0, 255, 0, 1);
	float fade = abs(cos(time * 0.1 * 3.1415 * 2));
	varColour = Fin;
	varColour.x = fade;
	return varColour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 lightColour;
float4 Fin = Texture(input.normal, input.fogFactor, input.tex);
float attenuation = 1 / (constFac + (linFac * dist) + (quad * pow(dist, 2)));

lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
lightColour *= attenuation;

if (change == 1) {
	Fin = changeColour(Fin);
}

float4 specular = calcSpecular(-lightDirection, input.normal, input.viewVector, specularColour, specularPower);

Fin.a = alpha;
lightColour.a = 1.f;

//if the light should be rendered render the texture plus lighting add specular
if (directional == 1) {
	float4 final = (lightColour * Fin) + specular;
	final.a = alpha;
	return final;
}
else
{
	//else return the texture
	return Fin;
}
}



