#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

class terrainGrayShader : public BaseShader
{
private:
	struct LightBufferType
	{
		float blend;   //blend between textures
		XMFLOAT3 padding;
	};

public:
	terrainGrayShader(ID3D11Device* device, HWND hwnd);
	~terrainGrayShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, float blend);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	//shader buffers
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

