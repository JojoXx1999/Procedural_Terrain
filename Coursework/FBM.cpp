#include "FBM.h"
#include "SimplexNoise.h"
#include <cmath>

//set initial variables
float FractionalBrownianMotion::frequency = 1;
float FractionalBrownianMotion::amplitude = 1;
float FractionalBrownianMotion::lacunarity = 2.0f;
float FractionalBrownianMotion::gain = 1.0f / FractionalBrownianMotion::lacunarity;

FractionalBrownianMotion::FractionalBrownianMotion() {}


FractionalBrownianMotion::~FractionalBrownianMotion() {}


float FractionalBrownianMotion::FBm(float x, float y, int octaves, float pelinFrequancy, bool useAbs, float seed)
{
	//use user inputs to change fbm
	float output = 0.f;
	float totalAmp = 0.f;
	float frequency = FractionalBrownianMotion::frequency;
	float amplitude = FractionalBrownianMotion::amplitude;

	//for the number of octaves, that is the number of times the noise will be applied
	for (int i = 0; i < octaves; i++)
	{
		//if the user wants to use the absolute value, to have no negatives
		if (useAbs)
		{
			//set the output to the amplitude multiplied by the absolute simplex noise
			output += (amplitude * abs(SimplexNoise::noise(x * frequency, y * frequency, seed, pelinFrequancy)));

		}
		else
		{
			//set the output to the amplitude multiplied by simplex noise
			output += (amplitude * SimplexNoise::noise(x * frequency, y * frequency, seed, pelinFrequancy));
		}
		//increase current the amplitude
		totalAmp += amplitude;

		//change frequency and amplitude
		frequency *= lacunarity;
		amplitude *= gain;
	}

	return (output / totalAmp);
}