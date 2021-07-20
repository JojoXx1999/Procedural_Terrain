#include "TerrainMeshExample.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "App1.h"
#include <fstream>
#include "CPerlinNoise.h"
#include <iostream>
#include <string>
#include "DiamonSquare.h"

//#pragma warning(disable:4996)

TerrainMeshExample::TerrainMeshExample(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int lresolution) :
	PlaneMesh(device, deviceContext, lresolution)
{

	//diamondSquareRange = 150;

	waterMesh = new PlaneMesh(device, deviceContext, 149);


	srand(time(NULL));
	vertexBuffer = NULL;
	resolution = 149;
	Resize(resolution);
	heightMap = new float[resolution * resolution];
	BuildHeightMap(0.f, 0.f);
	RandomTerrain(0);
	Regenerate(device, deviceContext, startH, startF);
	setTex(0);
	setBlendRate(0.2);
}

//Function which simply utilises Perlin Noise
void TerrainMeshExample::perlinNoise(float pScale, float pSize) {
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			float scale = pScale;
			float sampleX = i + 0.00187;
			float sampleY = j + 0.00187;

			sampleX *= scale;
			sampleY *= scale;

			float sampler[2] = { sampleX, sampleY };
			float perlinValue1 = CPerlinNoise::noise2(&sampler[0]);

			heightMap[(i * resolution) + (j)] += (perlinValue1 *pSize);
		}
	}
}

//Function which utilises Simplex Noise
void TerrainMeshExample::simplexNoise(float pScale, float pSize) {
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			float scale = pScale;
			float sampleX = i + 0.00187;
			float sampleY = j + 0.00187;

			sampleX *= scale;
			sampleY *= scale;

			float sampler[2] = { sampleX, sampleY };
			float perlinValue1 = CPerlinNoise::noise2(&sampler[0]);
			float simplex = SimplexNoise::noise(sampleX, sampleY);

			heightMap[(i * resolution) + (j)] += (simplex * pSize);
		}
	}
}

//function to use fractional brownian motion
void TerrainMeshExample::fbm(int octaves, float f, bool abs) {
	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			float fbm = fbmPtr.FBm(i, j, octaves, f, abs, rand() % 10);

			heightMap[(i * resolution) + (j)] += fbm;
		}
	}
}


//Cleanup the heightMap
TerrainMeshExample::~TerrainMeshExample() {
	delete[] heightMap;
	heightMap = 0;
}

//set corners for Diamon-Sqaure algorithm
void TerrainMeshExample::setCorners() {

	DS.setCorners(heightMap);
}

//Resize Height Map
void TerrainMeshExample::Resize(int newResolution) {
	resolution = newResolution;
	heightMap = new float[resolution * resolution];
	if (vertexBuffer != NULL) {
		vertexBuffer->Release();
	}
	vertexBuffer = NULL;
}

//Create random terrain using a combination of techniques
void TerrainMeshExample::RandomTerrain(int detail) {
	CPerlinNoise* n;
	BuildHeightMap(0, 0);
	getHighest();
	for (int i = 0; i < 100; i++) {

		float randF = rand() % 4;
		float randS = rand() % 3;

		Fault(randF);
		for (int j = 0; j <= detail; j++) {
			Particles(1.5);
		}
		if (i % 2 == 0) {
			SmoothTerrain(randS);
		}
		RandomNoise();

	}
	for (int i = 0; i < 60; i++) {

		if (i % 12 == 0) {
			DS.setCorners(heightMap);
		}
		SmoothTerrain(4);
	}
}

//calculate the climate for the terrain using the height
float TerrainMeshExample::getClimate(float h, bool b, float s) {
	if (b == false) {
		float Height = h;
		double percent = 20000;
		climate = h / percent;
	}
	else {
		climate = s;
	}

	//clamp between 0 and 1
	if (climate < 0) { climate = 0; }
	else if (climate > 1) { climate = 1; }
	return climate;
}

//set the climate by user input
void TerrainMeshExample::setClimate(float c) {
	climate = c;
}

//find the highest point on the plane
float TerrainMeshExample::getHighest() {
	float highestHeight = -999.f;
	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			if (heightMap[((j)* resolution) + (i)] > highestHeight) {
				highestHeight = heightMap[((j)* resolution) + (i)];
			}
		}
	}
	return highestHeight;
}

//find the lowest point on the plane
float TerrainMeshExample::getLowest() {
	float lowestHeight = 999.f;
	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			if (heightMap[((j)* resolution) + (i)] < lowestHeight) {
				lowestHeight = heightMap[((j)* resolution) + (i)];
			}
		}
	}
	return lowestHeight;
}

//Centre the lowest point to 0 (move the terrain)
void TerrainMeshExample::MoveHeightMap() {
	float lowest = getLowest();
	float change = 0 - lowest;

	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			(heightMap[((j)* resolution) + (i)]) += change;
		}
	}
}

//Create random noise
void TerrainMeshExample::RandomNoise() {

	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			float random = rand() % (2 + 1);
			float randomH = rand() % (1);
			(random == 2 ? heightMap[(j * resolution) + i] += randomH : heightMap[(j * resolution) + i]);
		}
	}
}

//Make all points on the hieght map positive
void TerrainMeshExample::AbsoluteHeightMap() {
	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			heightMap[(j * resolution) + i] = abs(heightMap[(j * resolution) + i]);
		}
	}
}


//Fill an array of floats that represent the height values at each grid point.
//Here we are producing a Sine wave along the X-axis
void TerrainMeshExample::BuildHeightMap(float h, float f) {
	for (int j = 0; j < (resolution); j++) {


		for (int i = 0; i < (resolution); i++) {
			//heightMap[(j * resolution) + i] = ((sin((float)i*f)) * h) + ((sin((float)i*(f * 2))) * (h / 2));
			heightMap[(j * resolution) + i] = 0;
		}
	}
}

void TerrainMeshExample::ChangeHeightMap(int h) {

	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			if (heightMap[(j * resolution) + i] < 0)
				heightMap[(j * resolution) + i] += h;
			else if (heightMap[(j * resolution) + i] > 0)

				heightMap[(j * resolution) + i] -= (float)h / 2;
		}
	}

}

//Smooth the terrain by taking the average height of each vertex's neighbours
void TerrainMeshExample::SmoothTerrain(int smooth) {
	int smoothingRate = smooth + 1;
	mapCopy = heightMap;
	for (int j = 0; j < (resolution); j++) {
		for (int i = 0; i < (resolution); i++) {
			float avgHeight = 0; int count = 0;

			if ((i - 1) >= 0) {
				if ((j) < (resolution - 1)) {
					avgHeight += mapCopy[(j * resolution) + (i - 1)];
					count++;
				}
				if ((j - 1) >= 0) {
					avgHeight += mapCopy[((j - 1) * resolution) + (i - 1)];
					count++;
				}
			}
			if ((i) < (resolution - 1)) {
				if ((j) < (resolution - 1)) {
					avgHeight += mapCopy[(j * resolution) + i];
					count++;
				}
				if ((j - 1) >= 0) {
					avgHeight += mapCopy[((j - 1) * resolution) + i];
					count++;
				}
			}
			avgHeight /= count;
			heightMap[(j * resolution) + i] -= ((heightMap[(j * resolution) + i] - avgHeight) / smoothingRate);

		}
	}
}

//Create particles which will be placed if it's neighbours are not at a lower height
void TerrainMeshExample::Particles(float p) {

	for (int k = 0; k < 20; k++) {
		int randomX = rand() % ((resolution - 1) + 1);
		int randomY = rand() % ((resolution - 1) + 1);
		XMINT2 location = { randomX,randomY };
		float current, next;
		current = heightMap[(randomY * resolution) + randomX];
		for (int j = randomY; j < (resolution); j++) {
			for (int i = randomX; i < (resolution); i++) {

				if ((i - 1) >= 0) {
					if ((j) < (resolution - 1)) {
						next = heightMap[(j * resolution) + (i - 1)];
						if (current > next) { location.x = (i - 1); location.y = (j); current = next; }
					}
					if ((j - 1) >= 0) {
						next = heightMap[((j - 1) * resolution) + (i - 1)];
						if (current > next) { location.x = (i - 1); location.y = (j - 1); current = next; }
					}
				}
				if ((randomX) < (resolution - 1)) {
					if ((j) < (resolution - 1)) {
						next = heightMap[(j * resolution) + i];
						if (current > next) { location.x = (i); location.y = (j); current = next; }
					}
					if ((randomY - 1) >= 0) {
						next = heightMap[((j - 1) * resolution) + i];
						if (current > next) { location.x = (i); location.y = (j - 1); current = next; }
					}
				}

			}
		}

		heightMap[(location.y * resolution) + location.x] += p;

	}
}

//create fault lines for big changes in height
void TerrainMeshExample::Fault(float f)
{	// define fault line
	int xDirection = rand() % 200 - 100; int xPoint = rand() % 100 - 1;
	int zDirection = rand() % 200 - 100; int zPoint = rand() % 100 - 1;
	XMFLOAT3 faultLine(xDirection, 0, zDirection);

	for (int j = 0; j < resolution; j++) {
		for (int i = 0; i < resolution; i++) {	// draw line from fault line to vert
			XMFLOAT3 VertexLine(i - xPoint, 0, j - zPoint);		// perform cross product
			float Y = (faultLine.z * VertexLine.x) - (faultLine.x * VertexLine.z);// if y pos then lift, else lower
			heightMap[(j * resolution) + i] += (Y > 0) ? (f) : (-f);
		}
	}
}

// Set up the heightmap and create or update the appropriate buffers
//
void TerrainMeshExample::Regenerate(ID3D11Device * device, ID3D11DeviceContext * deviceContext, float h, float f) {

	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, height, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Calculate and store the height values


	// Calculate the number of vertices in the terrain mesh.
	// We share vertices in this mesh, so the vertex count is simply the terrain 'resolution'
	// and the index count is the number of resulting triangles * 3 OR the number of quads * 6
	vertexCount = resolution * resolution;

	indexCount = ((resolution - 1) * (resolution - 1)) * 6;
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	index = 0;

	// UV coords.
	u = 0;
	v = 0;
	increment = m_UVscale / resolution;

	//Set up vertices
	for (j = 0; j < (resolution); j++) {
		for (i = 0; i < (resolution); i++) {
			positionX = (float)i;
			positionZ = (float)(j);

			height = heightMap[index];
			vertices[index].position = XMFLOAT3(positionX, height, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);

			u += increment;
			index++;
		}
		u = 0;
		v += increment;
	}

	//Set up index list
	index = 0;
	for (j = 0; j < (resolution - 1); j++) {
		for (i = 0; i < (resolution - 1); i++) {

			//Build index array
			indices[index] = (j*resolution) + i;
			indices[index + 1] = ((j + 1) * resolution) + (i + 1);
			indices[index + 2] = ((j + 1) * resolution) + i;

			indices[index + 3] = (j * resolution) + i;
			indices[index + 4] = (j * resolution) + (i + 1);
			indices[index + 5] = ((j + 1) * resolution) + (i + 1);
			index += 6;
		}
	}

	//Set up normals
	for (j = 0; j < (resolution - 1); j++) {
		for (i = 0; i < (resolution - 1); i++) {
			//Calculate the plane normals
			XMFLOAT3 a, b, c;	//Three corner vertices
			a = vertices[j * resolution + i].position;
			b = vertices[j * resolution + i + 1].position;
			c = vertices[(j + 1) * resolution + i].position;

			//Two edges
			XMFLOAT3 ab(c.x - a.x, c.y - a.y, c.z - a.z);
			XMFLOAT3 ac(b.x - a.x, b.y - a.y, b.z - a.z);

			//Calculate the cross product
			XMFLOAT3 cross;
			cross.x = ab.y * ac.z - ab.z * ac.y;
			cross.y = ab.z * ac.x - ab.x * ac.z;
			cross.z = ab.x * ac.y - ab.y * ac.x;
			vertices[j * resolution + i].normal = cross;
		}
	}

	//Smooth the normals by averaging the normals from the surrounding planes
	XMFLOAT3 smoothedNormal(0, 1, 0);
	for (j = 0; j < resolution; j++) {
		for (i = 0; i < resolution; i++) {
			smoothedNormal.x = 0;
			smoothedNormal.y = 0;
			smoothedNormal.z = 0;
			float count = 0;
			//Left planes
			if ((i - 1) >= 0) {
				//Top planes
				if ((j) < (resolution - 1)) {
					smoothedNormal.x += vertices[j * resolution + (i - 1)].normal.x;
					smoothedNormal.y += vertices[j * resolution + (i - 1)].normal.y;
					smoothedNormal.z += vertices[j * resolution + (i - 1)].normal.z;
					count++;
				}
				//Bottom planes
				if ((j - 1) >= 0) {
					smoothedNormal.x += vertices[(j - 1) * resolution + (i - 1)].normal.x;
					smoothedNormal.y += vertices[(j - 1) * resolution + (i - 1)].normal.y;
					smoothedNormal.z += vertices[(j - 1) * resolution + (i - 1)].normal.z;
					count++;
				}
			}
			//right planes
			if ((i) <(resolution - 1)) {

				//Top planes
				if ((j) < (resolution - 1)) {
					smoothedNormal.x += vertices[j * resolution + i].normal.x;
					smoothedNormal.y += vertices[j * resolution + i].normal.y;
					smoothedNormal.z += vertices[j * resolution + i].normal.z;
					count++;
				}
				//Bottom planes
				if ((j - 1) >= 0) {
					smoothedNormal.x += vertices[(j - 1) * resolution + i].normal.x;
					smoothedNormal.y += vertices[(j - 1) * resolution + i].normal.y;
					smoothedNormal.z += vertices[(j - 1) * resolution + i].normal.z;
					count++;
				}
			}
			smoothedNormal.x /= count;
			smoothedNormal.y /= count;
			smoothedNormal.z /= count;

			float mag = sqrt((smoothedNormal.x * smoothedNormal.x) + (smoothedNormal.y * smoothedNormal.y) + (smoothedNormal.z * smoothedNormal.z));
			smoothedNormal.x /= mag;
			smoothedNormal.y /= mag;
			smoothedNormal.z /= mag;

			vertices[j * resolution + i].normal = smoothedNormal;
		}
	}
	//If we've not yet created our dyanmic Vertex and Index buffers, do that now
	if (vertexBuffer == NULL) {
		CreateBuffers(device, vertices, indices);
	}
	else {
		//If we've already made our buffers, update the information
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//  Disable GPU access to the vertex buffer data.
		deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		//  Update the vertex buffer here.
		memcpy(mappedResource.pData, vertices, sizeof(VertexType) * vertexCount);
		//  Reenable GPU access to the vertex buffer data.
		deviceContext->Unmap(vertexBuffer, 0);
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

//Create the vertex and index buffers that will be passed along to the graphics card for rendering
//For CMP305, you don't need to worry so much about how or why yet, but notice the Vertex buffer is DYNAMIC here as we are changing the values often
void TerrainMeshExample::CreateBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices) {

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the dyanmic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
}

