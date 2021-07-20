//shader for manipulating the water mesh
#pragma once
#include "DXF.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

class WaterShader : public BaseShader
{
private:
	struct WaterBufferType
	{
		//variables for lighting calculations
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float alpha;
		XMFLOAT3 padding;
		float fogStrength;
		XMFLOAT4 bg;
		float directional;
		float spot;
		float pointL;
		float time;
		float change;
		XMFLOAT3 padding2;
		//variables for specular light
		float constFac;
		float linFac;
		float dist;
		float quad;
		float specularPower;
		XMFLOAT3 padding3;
		XMFLOAT4 specularColour;
	};

	struct FogBufferType
	{
		float fogStart;
		float fogEnd;
		float padding1, padding2;
	};

	struct TessellationBufferType
	{
		XMFLOAT3 cameraPos;
		float tessellation;
	};

	struct TimeBufferType
	{
		//variables for maniuplating the vertices
		float time;
		float amplitude;
		float frequency;
		float speed;
		XMFLOAT3 padding;
		float height;
		XMFLOAT3 camPos;
		float padding2;
	};

public:
	WaterShader(ID3D11Device* device, HWND hwnd);
	~WaterShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, Light* light, float delta, float amplitude, float frequency, float speed, float a, float h, float fogS, float fogE, float strength, XMFLOAT4 bg, int directional, int changeColour, float tessellation, XMFLOAT3 camPos, XMFLOAT4 specColour, float spec, float constFac, float linFac, float dist, float quad, XMFLOAT3 cameraPos);

private:
	//normal for tessellation
	void initShader(const wchar_t* cs, const wchar_t* hs, const wchar_t* ds, const wchar_t* ps);
	//normal function
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	//shader buffers
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* waterBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* fogBuffer;
	ID3D11Buffer* tessellationBuffer;
};

