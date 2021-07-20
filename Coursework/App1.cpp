// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"
#include "CPerlinNoise.h"
#include "TerrainMeshExample.h"
#include "DiamonSquare.h"
#include "GenerateName.h"
#include "CAShader.h"
#include "HorizontalBlur.h"
#include "FBM.h"
#include "Bloom.h"

App1::App1() :
	lSystem("A")
{
	m_Plane = nullptr;
	m_water = nullptr;
	terrainShader = nullptr;
	waterShader = nullptr;
	m_Line = nullptr;
}


void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);
	textureMgr->loadTexture(L"top", L"res/snow.jpg");
	textureMgr->loadTexture(L"mid", L"res/snowyMountain.png");
	textureMgr->loadTexture(L"low", L"res/grass.jpg");
	textureMgr->loadTexture(L"water", L"res/water.jpg");
	textureMgr->loadTexture(L"sand", L"res/sand.jpg");
	textureMgr->loadTexture(L"soil", L"res/Rock2.jpg");
	textureMgr->loadTexture(L"sky", L"res/sky.png");

	// Create Mesh object and shader object
	m_Plane = new TerrainMeshExample(renderer->getDevice(), renderer->getDeviceContext());
	m_water = new Water(renderer->getDevice(), renderer->getDeviceContext());
	//waterMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	terrainShader = new LightShader(renderer->getDevice(), hwnd);
	waterShader = new WaterShader(renderer->getDevice(), hwnd);
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	horizontalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	bloomTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	CATexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	shadowTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	depthShader2 = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	terrainGrey = new terrainGrayShader(renderer->getDevice(), hwnd);
	fogShader = new FogShader(renderer->getDevice(), hwnd);
	treeShader = new LightShader(renderer->getDevice(), hwnd);
	lightPos = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	skyBox = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	CAshader = new CA(renderer->getDevice(), hwnd);
	HB = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	VB = new VerticalBlurShader(renderer->getDevice(), hwnd);
	Bloom = new bloom(renderer->getDevice(), hwnd);

	current = renderTexture;
	//m_water = new TessellationMesh(renderer->getDevice(), renderer->getDeviceContext());

	//TessellationMesh
	int shadowmapWidth = 2048;
	int shadowmapHeight = 2048;
	int sceneWidth = 700;
	int sceneHeight = 700;

	shadowMap = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);

	//directional light set up
	light = new Light;
	light->setAmbientColour(0.25f, 0.25f, 0.25f, 1.0f);
	light->setDiffuseColour(lightDColour.x, lightDColour.y, lightDColour.z, lightDColour.w);
	light->setDirection(1.f, -0.4f, 0.0f);

	//main let set up
	light2 = new Light;
	light2->setAmbientColour(0.25f, 0.25f, 0.25f, 1.0f);
	light2->setDiffuseColour(lightDColour.x, lightDColour.y, lightDColour.z, lightDColour.w);
	light2->setDirection(lightDirection.x, lightDirection.y, lightDirection.z);
	light2->setPosition(0.f, 0.f, 0.f);
	light2->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	//point light set up
	pointLight = new Light;
	pointLight->setAmbientColour(0.25, 0.25, 0.25, 1.f);
	pointLight->setDiffuseColour(lightPColour.x, lightPColour.y, lightPColour.z, 1.f);
	pointLight->setDirection(lightPDirection.x, lightPDirection.y, lightPDirection.z);
	pointLight->setPosition(0.f, 0.f, 0.f);
	pointLight->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	camera->setPosition(0.0f, 45.0f, -30.0f);
	camera->setRotation(0.0f, 30.0f, 30.0f);

	terrainResolution = m_Plane->GetResolution();


	m_Line = new LineMesh(renderer->getDevice(), renderer->getDeviceContext());


}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_Plane)
	{
		delete m_Plane;
		m_Plane = 0;
	}
	if (m_water)
	{
		delete m_water;
		m_water = 0;
	}
	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = 0;
	}
	if (waterShader)
	{
		delete waterShader;
		waterShader = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}
	delta += timer->getTime();

	return true;
}

bool App1::render()
{
	genName.AddDes(startBlend);


	firstPass();

	secondPass();

	horizontal();
	vertical();

	Chromatic();

	finalPass();

	return true;
}

void App1::firstPass() {
	//render the scene to a render to texture
	feet = (((int)m_Plane->getHighest() - startWater) * ((int)4413 / 70));
	startBlend = m_Plane->getClimate(feet, overwrite, startBlend);
	renderTexture->setRenderTarget(renderer->getDeviceContext());
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), bg.x, bg.y, bg.z, 1.0f);

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	m_Plane->sendData(renderer->getDeviceContext());
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"mid"), textureMgr->getTexture(L"low"), textureMgr->getTexture(L"top"), textureMgr->getTexture(L"soil"), textureMgr->getTexture(L"sand"), light2, pointLight, startBlend, m_Plane->getHighest(), m_Plane->getLowest(), startWater, directional, fogS, fogE, fogStrength, bg);
	terrainShader->render(renderer->getDeviceContext(), m_Plane->getIndexCount());

	renderer->setAlphaBlending(true);
	m_water->sendData(renderer->getDeviceContext());
	waterShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"water"), light2, delta, Astart, Fstart, Sstart, alphaStart, startWater, fogS, fogE, fogStrength, bg, (int)directional, (int)changeColour, tessellation, camera->getPosition(), specColour, spec, constFac, linFac, dist, quad, camera->getPosition());
	waterShader->render(renderer->getDeviceContext(), m_water->getIndexCount());
	renderer->setAlphaBlending(false);

	int h = m_Plane->getTexture();

	//calculate height of the terrain
	m_Plane->sendData(renderer->getDeviceContext());
	feet = (((int)m_Plane->getHighest() - startWater) * ((int)4413 / 70));
	startBlend = m_Plane->getClimate(feet, overwrite, startBlend);
	renderer->setBackBufferRenderTarget();

}

void App1::secondPass() {
	//Create depth map of the terrain
	shadowMap->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	light2->generateViewMatrix();
	XMMATRIX lightViewMatrix = light2->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light2->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	m_Plane->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), m_Plane->getIndexCount());

	pointLight->generateViewMatrix();
	lightViewMatrix = pointLight->getViewMatrix();
	lightProjectionMatrix = pointLight->getOrthoMatrix();
	worldMatrix = renderer->getWorldMatrix();

	m_Plane->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), m_Plane->getIndexCount());


	renderer->resetViewport();
	renderer->setBackBufferRenderTarget();
}

void App1::Chromatic() {
	//Create the chromatic aberration post process and render it in a render to texture
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	CATexture->setRenderTarget(renderer->getDeviceContext());
	CATexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = CATexture->getOrthoMatrix();

	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	CAshader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, current->getShaderResourceView(), ++time, rgb, CAfrequency);
	CAshader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	renderer->setBackBufferRenderTarget();
}


void App1::horizontal()
{
	//Create the horizontal blur post process and render it in a render to texture
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeX = (float)horizontalBlurTexture->getTextureWidth();
	horizontalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	horizontalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = horizontalBlurTexture->getOrthoMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	HB->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, current->getShaderResourceView(), screenSizeX);
	HB->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::vertical()
{
	//Create the vertical blur post process and render it in a render to texture
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)verticalBlurTexture->getTextureHeight();
	verticalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	verticalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 1.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = verticalBlurTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	//pass in the horizontal blur render to texture so the two blurs combine
	VB->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, horizontalBlurTexture->getShaderResourceView(), screenSizeY);
	VB->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::finalPass() {
	//render the scene
	int h = m_Plane->getTexture();
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	renderer->beginScene(bg.x, bg.y, bg.z, 1.f);



	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	worldMatrix = XMMatrixTranslation(startPPos.x, startPPos.y, startPPos.z);

	worldMatrix = renderer->getWorldMatrix();
	// Send geometry data, set shader parameters, render object with shader


	renderer->setAlphaBlending(false);
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	m_Plane->sendData(renderer->getDeviceContext());

	if (shadows) {
		//If shadows are selected render the to a render to texture
		shadowTexture->setRenderTarget(renderer->getDeviceContext());
		shadowTexture->clearRenderTarget(renderer->getDeviceContext(), bg.x, bg.y, bg.z, 1.0f);

		m_Plane->sendData(renderer->getDeviceContext());
		shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
			textureMgr->getTexture(L"mid"), textureMgr->getTexture(L"low"), textureMgr->getTexture(L"top"), textureMgr->getTexture(L"soil"), textureMgr->getTexture(L"sand"), shadowMap->getDepthMapSRV(),
			light2, startBlend, m_Plane->getHighest(), m_Plane->getLowest(), startWater, bias);
		shadowShader->render(renderer->getDeviceContext(), m_Plane->getIndexCount());

		renderer->setAlphaBlending(true);
		m_water->sendData(renderer->getDeviceContext());
		waterShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"water"), light2, delta, Astart, Fstart, Sstart, alphaStart, startWater, fogS, fogE, fogStrength, bg, (int)directional, (int)changeColour, tessellation, camera->getPosition(), specColour, spec, constFac, linFac, dist, quad, camera->getPosition());
		waterShader->render(renderer->getDeviceContext(), m_water->getIndexCount());
		renderer->setAlphaBlending(false);
		renderer->setBackBufferRenderTarget();
		current = shadowTexture;
	}
	else {
		//otherwise set to the render to texture from the first pass
		current = renderTexture;
	}

	//render grey scale
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	terrainGrey->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, current->getShaderResourceView(), greyBlend);
	terrainGrey->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setBackBufferRenderTarget();

	if (ca == true) {
		//render chromatic abberation ontop
		terrainGrey->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, CATexture->getShaderResourceView(), greyBlend);
		terrainGrey->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
		renderer->setBackBufferRenderTarget();

	}
	//or render blur ontop
	else if (blur == true) {
		terrainGrey->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, verticalBlurTexture->getShaderResourceView(), greyBlend);
		terrainGrey->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	}
	renderer->setZBuffer(true);

	//	renderer->resetViewport();
	renderer->setBackBufferRenderTarget();

	/*
	------------------------------------------Fog--------------------------------------------
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	fogShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,fogS, fogE, fogStrength, renderTexture->getShaderResourceView());
	fogShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);
	*/

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();
}

void App1::gui()
{
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------Set Up-------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);
	// Build UI
	string height = std::to_string(feet);
	currentHeight = feet;
	//AddGeo(currentHeight);
	genName.AddGeo(currentHeight);
	ImGui::StyleColorsClassic();
	ImGui::SetWindowFontScale(1.5);
	name = genName.getName();
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------Miscellaneous Controls-----------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	const char *cstr = name.c_str();
	ImGui::TextColored(ImVec4(1.f, 0.f, 0.6f, 1.f), "%s", cstr);

	const char* cstrh = height.c_str();
	ImGui::TextColored(ImVec4(0.f, 0.6f, 0.6f, 1.f), "%s ft", cstrh);
	ImGui::SetWindowFontScale(1);
	ImGui::Text(" ");
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Text("Camera Pos: (%.2f, %.2f, %.2f)", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	if (ImGui::Button("Change Name")) {
		genName.Reset();
		genName.ChangeName(startBlend, feet);
	}

	if (ImGui::Button("Randomise")) {
		genName.Reset();
		genName.MakeName();
		m_Plane->BuildHeightMap(0, 0);
		setSmooth(0);
		setFault(0);
		m_Plane->RandomTerrain(getDetail());
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//


	ImGui::Text("");
	ImGui::Text("Post Processing Settings: ");
	ImGui::SliderFloat("Grey Scale strength", &greyBlend, 0.f, 1.f);

	(ImGui::Checkbox("Chromatic Aberration", &ca));
	(ImGui::Checkbox("Blur", &blur));
	ImGui::SliderFloat3("Chromatic Aberration Colour", &rgb.x, 0.f, 1.f);
	ImGui::SliderFloat3("Chromatic Aberration Frequency", &CAfrequency.x, 0.f, 1.f);
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------Lighting Controls-------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	ImGui::Text("");
	ImGui::Text("Light Settings: ");
	(ImGui::Checkbox("Directional Light", &directional));
	(ImGui::Checkbox("Point Light", &pointLightToggle));
	ImGui::Checkbox("Draw Shadows", &shadows);
	ImGui::SliderFloat("Shadow Bias", &bias, 0.f, 0.1f);


	if (pointLightToggle == false) {
		pointLight->setDiffuseColour(0.f, 0.f, 0.f, 0.f);
	}
	else
	{
		pointLight->setDiffuseColour(lightPColour.x, lightPColour.y, lightPColour.z, 1.f);
	}

	if (ImGui::ColorEdit4("Light1 Diffuse Light Colour", &lightDColour.x)) {
		light->setDiffuseColour(lightDColour.x, lightDColour.y, lightDColour.z, 1);
		light2->setDiffuseColour(lightDColour.x, lightDColour.y, lightDColour.z, 1);
	}
	if (ImGui::ColorEdit4("Light2 Diffuse Light Colour", &lightPColour.x)) {
		pointLight->setDiffuseColour(lightPColour.x, lightPColour.y, lightPColour.z, 1);
	}
	if (ImGui::ColorEdit4("water Specular Light Colour", &specColour.x)) {
		specColour = XMFLOAT4(specColour.x, specColour.y, specColour.z, 1);
	}
	ImGui::SliderFloat("water Specular Strength", &spec, 0, 200);

	if (ImGui::DragFloat3("Light Direction", &lightDirection.x, 0.1, -1, 1)) {
		light2->setDirection(lightDirection.x, lightDirection.y, lightDirection.z);

	}

	if (ImGui::DragFloat3("Light1 Position", &startDPos.x, 0, -100, 100)) {
		light2->setPosition(startDPos.x, startDPos.y, startDPos.z);
	}

	if (ImGui::DragFloat3("Light2 Light Position", &startPPos.x, 0, -100, 100)) {
		pointLight->setPosition(startPPos.x, startPPos.y, startPPos.z);
	}

	ImGui::SliderFloat("Attenuation Constant Factor", &constFac, 0, 1);
	ImGui::SliderFloat("Attenuation Linear Factor", &linFac, 0, 1);
	ImGui::SliderFloat("Attenuation Distance Factor", &dist, 0, 1);
	ImGui::SliderFloat("Attenuation Quad Factor", &quad, 0, 1);

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------Terrain Controls--------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	ImGui::Text("");
	ImGui::Text("Terrain Settings: ");

	(ImGui::Checkbox("Overwrite Climate", &overwrite));

	if (ImGui::SliderFloat("Climate", &startBlend, 0, 1)) {
		m_Plane->setClimate(startBlend);
	}

	if (ImGui::SliderInt("Smooth Rate", &startS, 0, 50)) {
		setSmooth(startS);
	}

	if (ImGui::SliderInt("Fault Rate", &startFault, 0, 5)) {
		setFault(startFault);
	}

	if (ImGui::SliderFloat("Particle Size", &startP, 0, 20)) {
		setP(startP);
	}

	if (ImGui::SliderInt("Random Terrain Detail Level", &startDetail, 0, 100)) {

		setDetail(startDetail);
	}

	ImGui::SliderFloat("Perlin Scale", &pScale, 0, 0.1);
	ImGui::SliderFloat("Perlin Size", &pSize, 0.1, 10);

	(ImGui::Checkbox("FBM absolute", &absolute));
	ImGui::SliderInt("FBM Octaves", &octaves, 1, 10);
	ImGui::SliderFloat("FBM Frequency", &fbmF, 0.1, 10);

	if (ImGui::IsKeyDown(90)) { //when z is pressed
		setSmooth(startS);
		m_Plane->SmoothTerrain(getSmooth());

		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("Fault")) {
		m_Plane->Fault(getFault());
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}
	else if (ImGui::IsKeyDown(70)) { //when f is pressed
		m_Plane->Fault(getFault());
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::IsKeyDown(80)) { //when p is pressed
		m_Plane->Particles(getP());
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}
	if (ImGui::Button("Particles")) {
		m_Plane->Particles(getP());
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("MidPlace Displacement")) {

		m_Plane->setCorners();
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());

	}

	if (ImGui::Button("Perlin Noise")) {
		m_Plane->perlinNoise(pScale, pSize);
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("Simplex Noise")) {
		m_Plane->simplexNoise(pScale, pSize);
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("FBM")) {
		m_Plane->fbm(octaves, fbmF, absolute);
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("Absolute")) {
		m_Plane->AbsoluteHeightMap();
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("Random Terrain")) {
		genName.Reset();
		genName.MakeName();
		m_Plane->BuildHeightMap(0, 0);
		setSmooth(0);
		setFault(0);
		m_Plane->RandomTerrain(getDetail());
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	if (ImGui::Button("Centre")) {
		m_Plane->MoveHeightMap();
		m_Plane->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), getHeight(), getFrequency());
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//

	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------Water Controls---------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	ImGui::Text("");
	ImGui::Text("Water Settings: ");
	ImGui::Checkbox("Cycle Colour", &changeColour);
	if (ImGui::SliderFloat("Water Level", &startWater, -100, 100)) {
		// m_water->Move(startWater);
	}
	ImGui::SliderFloat("Water Transparency", &alphaStart, 0, 1);
	ImGui::SliderFloat("Amplitude", &Astart, 0, 20);
	ImGui::SliderFloat("Frequency", &Fstart, 0, 1);
	ImGui::SliderFloat("Speed", &Sstart, 0, 20);
	//------------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------------//

	//------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------Render GUI----------------------------------------------------------------//
	//------------------------------------------------------------------------------------------------------------------------------------------//

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
//------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------End--------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------//
