#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class ReflectionShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ReflectionBufferType
	{
		XMMATRIX reflectionMatrix;
	};

public:

	ReflectionShader(ID3D11Device* device, HWND hwnd);
	~ReflectionShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture2);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* m_reflectionBuffer;
};

