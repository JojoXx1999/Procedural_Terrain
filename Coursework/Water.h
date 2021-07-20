//Header for water object plane
#pragma once
#include "PlaneMesh.h"
#include "TessellationMesh.h"
class Water :
	public PlaneMesh {
public:
	Water(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 100);
	~Water();

	void Regenerate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float height, float frequency);
	void BuildHeightMap(float height, float frequency);
	void CreateBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	const inline int GetResolution() { return resolution; }
	void Move(float adjust);
	void Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext);


private:
	const float m_UVscale = 10.0f;	//Tile the UV map 10 times across the plane
	float* heightMap;
	int resolution;
	float frequency = 0.1, startH = 0.2f, startF = 1.f;

};
