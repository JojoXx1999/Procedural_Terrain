// Light pixel shader
//get all of the useable textures
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
Texture2D texture4 : register(t4);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{

	float4 ambientColour[2];
	float4 diffuseColour[2];
	float4 position[1];
	float3 lightDirection;
	float padding;
	float4 bg;
	float directional;
	float height;
	float lowest;
	float waterHeight;
	float fogS;
	float fogE;
	float fogStrength;
	float blendRate;

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


//function for calculating whuch texture should be used
float4 calculateTexture(float4 tex1, float4 tex2, float4 tex3, float4 tex4, float4 tex5, float rate, float3 Pos, float r2)
{
	//top (snow) section
	float H = height - lowest;				//get range of height in the terrin			
	float snowRate = (H / 5) * (rate * 10); //decide where the snow texture should stop
	float range = (snowRate);
	float4 finalColor;
	float4 newColor;
	float4 color;
	int r = height / 100;		            //calculate blend rate of textures based on height		
	float snowLine = height - snowRate;		//get the range oh height the snow texture will cover

											//next (rock) section
	range = snowLine;
	float max = height - ((height - snowLine) / 3); //get the range of heights where the rock texture can appear
	float min = snowLine;							//based on the height of the snow	 
	float current = 1 / Pos.y;
	range = max - min;

	//next (grass) section
	r2 = (Pos.y - min) * (1.f / (max - min));       //calculate blend rate of textures based on height
	float minG = snowLine - 50;						//get the range of heights where the grass can appear
	float maxG = (snowLine);						//based on the height of the rock
	range = maxG - minG;
	float r3 = (Pos.y - minG) * (1.f / (maxG - minG)); //calculate blend rate of textures based on height

													   //choose (for the current pixel) which texture should be used
	if (r2 < 0) { r2 = 0; }
	else if (r2 > 1) { r2 = 1; }
	float minimum = minG - 20;
	float r4 = (Pos.y - minimum) * (1.f / (minG - minimum)); //calculate blend rate of textures based on height
	if (Pos.y < minimum) {
		finalColor = tex5; // render grass
	}
	else if (Pos.y <= minG && Pos.y >= minimum) {
		finalColor = lerp(tex5, tex2, r4); //render a mixture of grass and sand
	}
	else if (Pos.y < height - snowRate && Pos.y > minG) {
		finalColor = lerp(tex2, tex1, r3); //render a mixture of grass and rock
	}
	else if (Pos.y >= snowLine)
	{
		finalColor = lerp(tex1, tex3, r2); //render a mixture of rock and grass
	}

	if (Pos.y < waterHeight) {
		finalColor = tex5; //if the pixel is below the water render as sand
	}
	else if (Pos.y >= waterHeight && Pos.y < waterHeight + 1) {
		//otherwise render the selected texture(s)
		float blend = (Pos.y - (waterHeight + 1)) * (1.f / ((waterHeight)-(waterHeight + 1)));
		finalColor = lerp(finalColor, tex5, blend);
	}

	return (finalColor);
}


// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	//calculate lighting for the pixel
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	//float4 lightColor;
	float4 textureColour;
float4 textureColor;
float4 textureColor1;
float4 textureColor2;
float4 textureColor3;
float4 textureColor4;
float4 Color;
float4 Color2;
float rate2 = 1;
float blendAmount = 0;


// Sample the textures
textureColor = texture0.Sample(sampler0, input.tex);
textureColor1 = texture1.Sample(sampler0, input.tex);
textureColor2 = texture2.Sample(sampler0, input.tex);
textureColor3 = texture3.Sample(sampler0, input.tex);
textureColor4 = texture4.Sample(sampler0, input.tex);

textureColour = texture0.Sample(sampler0, input.tex);

//set up multiple lights
float3 lightVector;
float4 lightColor[2];
lightVector = normalize(position[0] - input.worldPosition);
lightColor[0] = ambientColour[1] + calculateLighting(lightVector, input.normal, diffuseColour[1]);
lightColor[1] = ambientColour[0] + calculateLighting(-lightDirection, input.normal, diffuseColour[0]);

//calculate the gradient of the pixel based on its normal
float slope = 1.f - input.normal.y;

//get the texture chosen by the calculateTexture function
Color = calculateTexture(textureColor, textureColor1, textureColor2, textureColor3, textureColor4, blendRate, input.worldPosition, rate2);

//if gradient is 0
if (slope == 0) {
	//render sand;
	Color2 = textureColor4;
}
if (slope < 0.5 && slope > 0)
{
	//if between range render the colour selected by the previous texture functiom
	Color2 = Color;
}

if ((slope < 0.5) && (slope >= 0.4)) {
	//if between range blend between dirt and the previous texture
	//blend based on steepness of gradient
	blendAmount = (slope - 0.4) * (1.f / (0.5 - 0.4));
	Color2 = lerp(Color, textureColor3, blendAmount);
}

if ((slope < 0.7) && (slope >= 0.5f))
{
	//if between range blend between soil and rock
	blendAmount = (slope - 0.5f) * (1.0f / (0.7f - 0.5f));
	Color2 = lerp(textureColor3, textureColor, blendAmount);
}

if (slope >= 0.7)
{
	//if the gradient is greater than or equal to 0.7 render rock
	Color2 = textureColor;
}

//calculate fog
float4 fogColor;
float4 finalColor;
fogColor = bg;
finalColor = (input.fogFactor * Color2 + (1.0 - input.fogFactor) * fogColor);

//set the final colour
float4 Fin = Color2;


//if the lighting should be rendered
if (directional == 1) {
	//return the lights and texture
	return (((lightColor[0] + lightColor[1])*(Fin)));
}
else
{
	//else only render the texture
	return (Fin);
}


}



