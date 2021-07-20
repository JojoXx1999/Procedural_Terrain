#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class bloom : public BaseShader
{
private:

public:
	bloom(ID3D11Device* device, HWND hwnd);
	~bloom();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2, float blend);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

