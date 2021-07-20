//-------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------By Jodie Duff----------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//
#include "DiamonSquare.h"																													 //
//-------------------------------------------------------------------------------------------------------------------------------------------//
DiamondSquare::DiamondSquare() {}																											 //
DiamondSquare::~DiamondSquare() {}																											 //
 //-------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------Set Corner Function------------------------------------------------------------//
void DiamondSquare::setCorners(float* heightMap) {																							 //

	float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1));
	heightMap[((resolution - 1) * resolution) + (resolution - 1)] += r;						//Set corner to random height -------------------//
	heightMap[(0 * resolution) + (resolution - 1)] += r;								    //Set corner to random height -------------------//
	heightMap[((resolution - 1) * resolution) + 0] += r;									//Set corner to random height -------------------//
	heightMap[(0 * resolution) + 0] += r;													//Set corner to random height -------------------//


	diamondSquare(heightMap, resolution, r);												//Go to diamond square function -----------------//
}																																			 //
//-------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------Diamond Step Function-----------------------------------------------------------//
void DiamondSquare::diamondStep(float* heightMap, int h, int l, int half, float rand)
{
	float avgHeight = 0;
	float count = 0;
	//for each point calculate the averge height of it's neighbours
	if ((l - 1) >= 0) {
		if ((h + 1) < (resolution)) {
			avgHeight += heightMap[((h + 1) * resolution) + (l - 1)];
			count++;
		}
		if ((h - 1) >= 0) {
			avgHeight += heightMap[((h - 1) * resolution) + (l - 1)];
			count++;
		}
	}
	if ((l + 1) < (resolution)) {
		if ((h + 1) < (resolution)) {
			avgHeight += heightMap[((h + 1) * resolution) + l];
			count++;
		}
		if ((h - 1) >= 0) {
			avgHeight += heightMap[((h - 1) * resolution) + l];
			count++;
		}
	}

	avgHeight /= count;

	heightMap[(h * resolution) + l] = avgHeight;
	heightMap[(h * resolution) + l] += rand;
}																																			 //
 //-----------------------------------------------------------Square Step Function------------------------------------------------------------//
void DiamondSquare::squareStep(float* heightMap, int h, int l, int half, float rand)
{
	float avgHeight = 0;
	float count = 0;
	//for each point calculate the averge height of it's neighbours
	if ((l - 1) >= 0) {
		if ((h + 1) < (resolution)) {
			avgHeight += heightMap[((h + 1) * resolution) + (l - 1)];
			count++;
		}
		if ((h - 1) >= 0) {
			avgHeight += heightMap[((h - 1) * resolution) + (l - 1)];
			count++;
		}
	}
	if ((l + 1) < (resolution)) {
		if ((h + 1) < (resolution)) {
			avgHeight += heightMap[((h + 1) * resolution) + l];
			count++;
		}
		if ((h - 1) >= 0) {
			avgHeight += heightMap[((h - 1) * resolution) + l];
			count++;
		}
	}
	avgHeight /= count;

	heightMap[(h * resolution) + l] = avgHeight;
	heightMap[(h * resolution) + l] += rand;
}																																			 //
 //----------------------------------------------------------Diamond Square Function----------------------------------------------------------//
void DiamondSquare::diamondSquare(float* heightMap, int size, float rand) {
	size -= 1;																				//Set size to minus 1 to stay within the range --//
	int half = size / 2;																	//Get the half size -----------------------------//
	if (half < 1) return;																	//If less than 1 stop running the function ------//
																							//
	for (int z = half; z < resolution; z += size)											//Perform Diamond Step --------------------------//
		for (int x = half; x < resolution; x += size)																						 //
			diamondStep(heightMap, x % resolution, z % resolution, half, rand);
	//
	int column = 0;																			//Variable to check odd or even columns ---------//
	for (int x = 0; x < resolution; x += half) {																							 //
		column++;																			//increment column ------------------------------//
		if (column % 2 == 1)																//If column is odd ------------------------------//
			for (int z = half; z < resolution; z += size)									//Perform square step using half terrain --------//
				squareStep(heightMap, x % resolution, z % resolution, half, rand);
		else                                                                                                                                 //
			for (int z = 0; z < resolution; z += size)										//Perform square step using full resolution -----//
				squareStep(heightMap, x % resolution, z % resolution, half, rand);
	}																																		 //
																																			 //
	size += 1;																				//Increment size --------------------------------//
	diamondSquare(heightMap, size / 2, rand / 1.1);											//Call function again ---------------------------//
}																																			 //
 //-------------------------------------------------------------------------------------------------------------------------------------------//