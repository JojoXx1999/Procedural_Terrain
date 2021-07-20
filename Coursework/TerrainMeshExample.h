#pragma once
#include "PlaneMesh.h"
#include <string>
#include "DiamonSquare.h"
#include "CPerlinNoise.h"
#include "FBM.h"
#include "SimplexNoise.h"

class TerrainMeshExample :
	public PlaneMesh {
public:
	TerrainMeshExample(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 149);
	~TerrainMeshExample();

	//Functions for procedural generation
	void Regenerate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float height, float frequency);
	void BuildHeightMap(float height, float frequency);
	void SmoothTerrain(int smooth);
	void Fault(float f);
	void RandomTerrain(int detail);
	void RandomNoise();
	void Particles(float p);
	void CreateBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	inline void setTex(int t) { texture = t; }
	inline int getTexture() { return texture; }
	inline static float random(int range) { return 0; }
	inline int getResolution() { return resolution; }
	void Resize(int newResolution);
	const inline int GetResolution() { return resolution; }
	void perlinNoise(float pScale, float pSize);
	void simplexNoise(float pScale, float pSize);
	void fbm(int octaves, float f, bool abs);
	void AbsoluteHeightMap();
	void ChangeHeightMap(int h);
	inline void setBlendRate(float r) { blend = r; }
	inline float getBlendRate() { return blend; }
	float getHighest();
	float getLowest();
	inline bool lowToHigh(int i, int j) { return (i < j); }
	void MoveHeightMap();
	float getClimate(float h, bool b, float s);
	void setClimate(float c);
	void setCorners();
	DiamondSquare DS;
	SimplexNoise pn;
	FractionalBrownianMotion fbmPtr;
	bool generateDiamondSquareTerrain;
private:

	const float m_UVscale = 10.0f;	//Tile the UV map 10 times across the plane
	float climate;
	float* heightMap;
	float* mapCopy;
	float frequency = 0.1, startH = 15.f, startF = 0.f;
	int numSquares = 4;
	int vertex = 4, texture, t;
	float blend = 0.f;
	XMFLOAT2 low = { 0, 100 };
	PlaneMesh* waterPlane;
	PlaneMesh* waterMesh;
	unsigned char* newTexture[150][150];
	float displacementHeight;
	int diamondSquareRange;
	int terrainHeight, terrainWidth;
	int* randomSeed;


};
