//-------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------By Jodie Duff----------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//
#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

class CA :											//Class for chromatic aberration
	public BaseShader
{
public:
	struct CABufferType
	{
		XMFLOAT3 rgb;								//float for offsetting rgb values
		float padding;
		XMFLOAT3 frequency;							//frequency fpr chromatic aberration
		float time;									//time used in conjunction with frequency

	};

	CA(ID3D11Device* device, HWND hwnd);
	~CA();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, float time, XMFLOAT3 rgb, XMFLOAT3 frequency);
private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

private:
	//shader buffers
	ID3D11Buffer * matrixBuffer;
	ID3D11Buffer* CABuffer;
	ID3D11SamplerState* sampleState;

	int time = 0;
};
//-------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------End---------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//
