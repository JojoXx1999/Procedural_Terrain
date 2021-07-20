#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		//arrays and variables for multiple lights
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 position[1];
		XMFLOAT3 direction;
		float padding;
		XMFLOAT4 bg;		//colour of background
		float directional;  //use directional light
		float height;		//heighest point on the terrain
		float lowest;		//lowest point on the terrain
		float startWater;
		float fogS;			//start position of the fog
		float fogE;			//end position of the fog
		float fogStrength;  //how much fog to add to the terrain
		float blend;

	};

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2, ID3D11ShaderResourceView* texture3, ID3D11ShaderResourceView* texture4, ID3D11ShaderResourceView* texture5, Light* light, Light* point, float bRate, float height, float lowest, float startWater, float directional, float fogS, float fogE, float fogStrength, XMFLOAT4 bg);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	//shader buffers
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* fogBuffer;
};

