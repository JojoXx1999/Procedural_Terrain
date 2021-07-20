#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

//Shader to add fog to terrain and water
class FogShader : public BaseShader
{
private:
	struct FogBufferType
	{
		//arrays and variables for multiple lights
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 position[1];
		XMFLOAT3 direction;
		float waterHeight;      //height of the water
		XMFLOAT4 bg;
		float directional;		//render main light
		float height;			//heighest point on terrain
		float lowest;			//lowest point on terrain
		float startWater;
		float fogS;				//fog start position
		float fogE;				//fog end position
		float fogStrength;		//how much fog to add to the terrain
		float blend;

	};

	struct FogVBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
	};

public:
	FogShader(ID3D11Device* device, HWND hwnd);
	~FogShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projectionMatrix, float fogS, float fogE, float fogStrength, ID3D11ShaderResourceView* texture);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	//shader buffers
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* fogBuffer;
};

