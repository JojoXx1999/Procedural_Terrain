#include "WaterShader.h"

WaterShader::WaterShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	//initShader(L"water_vs.cso", L"water_ps.cso");
	initShader(L"water_vs.cso", L"water_hs.cso", L"water_ds.cso", L"water_ps.cso");
}


WaterShader::~WaterShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (waterBuffer)
	{
		waterBuffer->Release();
		waterBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void WaterShader::initShader(const wchar_t* vsFilename, const wchar_t* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC waterBufferDesc;
	D3D11_BUFFER_DESC timeBufferDesc;
	D3D11_BUFFER_DESC fogBufferDesc;
	D3D11_BUFFER_DESC tesBufferDesc;


	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	waterBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	waterBufferDesc.ByteWidth = sizeof(WaterBufferType);
	waterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waterBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waterBufferDesc.MiscFlags = 0;
	waterBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&waterBufferDesc, NULL, &waterBuffer);

	fogBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fogBufferDesc.ByteWidth = sizeof(FogBufferType);
	fogBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogBufferDesc.MiscFlags = 0;
	fogBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&fogBufferDesc, NULL, &fogBuffer);

	tesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tesBufferDesc.ByteWidth = sizeof(TessellationBufferType);
	tesBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tesBufferDesc.MiscFlags = 0;
	tesBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&tesBufferDesc, NULL, &tessellationBuffer);

	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&timeBufferDesc, NULL, &timeBuffer);

}


void WaterShader::initShader(const wchar_t* vsFilename, const wchar_t* hsFilename, const wchar_t* dsFilename, const wchar_t* psFilename)
{

	// Load (+ compile) shader files
	loadVertexShader(hsFilename);
	loadPixelShader(dsFilename);
	initShader(vsFilename, psFilename);
}

void WaterShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, ID3D11ShaderResourceView* texture, Light* light, float delta, float amplitude, float frequency, float speed, float a, float h, float fogS, float fogE, float strength, XMFLOAT4 bg, int directional, int changeColour, float tessellation, XMFLOAT3 camPos, XMFLOAT4 specColour, float spec, float constFac, float linFac, float dist, float quad, XMFLOAT3 cameraPos)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//send to the time buffer in the vertex shader
	TimeBufferType* timePtr;
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	timePtr->time = delta;
	timePtr->amplitude = amplitude;
	timePtr->frequency = frequency;
	timePtr->speed = speed;
	timePtr->padding = XMFLOAT3(0.f, 0.f, 0.f);
	timePtr->height = h;
	timePtr->camPos = camPos;
	timePtr->padding2 = 0.f;
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &timeBuffer);

	//send to the fog buffer in the vertex shader
	FogBufferType* fogPtr;
	deviceContext->Map(fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	fogPtr = (FogBufferType*)mappedResource.pData;
	fogPtr->fogStart = fogS;
	fogPtr->fogEnd = fogE;
	fogPtr->padding1 = 0.f;
	fogPtr->padding2 = 0.f;
	deviceContext->Unmap(fogBuffer, 0);
	deviceContext->VSSetConstantBuffers(2, 1, &fogBuffer);

	//send to the hull shader in the vertex shader
	TessellationBufferType* tesPtr;
	deviceContext->Map(tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	tesPtr = (TessellationBufferType*)mappedResource.pData;
	tesPtr->tessellation = tessellation;
	tesPtr->cameraPos = camPos;
	deviceContext->Unmap(tessellationBuffer, 0);
	deviceContext->HSSetConstantBuffers(0, 1, &tessellationBuffer);

	//Additional
	// Send light data to pixel shader
	WaterBufferType* waterPtr;
	deviceContext->Map(waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	waterPtr = (WaterBufferType*)mappedResource.pData;
	waterPtr->diffuse = light->getDiffuseColour();
	waterPtr->direction = light->getDirection();
	waterPtr->alpha = a;
	waterPtr->padding = XMFLOAT3(0.f, 0.f, 0.f);
	waterPtr->fogStrength = strength;
	waterPtr->bg = bg;
	waterPtr->directional = directional;
	waterPtr->spot = 1.f;
	waterPtr->pointL = 1.f;
	waterPtr->time = delta;
	waterPtr->change = changeColour;
	waterPtr->padding2 = XMFLOAT3(0.f, 0.f, 0.f);
	waterPtr->constFac = constFac;
	waterPtr->linFac = linFac;
	waterPtr->dist = dist;
	waterPtr->quad = quad;
	waterPtr->specularPower = spec;
	waterPtr->padding3 = XMFLOAT3(0.f, 0.f, 0.f);
	waterPtr->specularColour = specColour;


	deviceContext->Unmap(waterBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &waterBuffer);



	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);

}
