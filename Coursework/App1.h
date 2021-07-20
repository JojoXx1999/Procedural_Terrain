//-------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------By Jodie Duff----------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//
#ifndef _APP1_H																																 //
#define _APP1_H																																 //
//------------------------------------------------------------------Includes-----------------------------------------------------------------//
#include "DXF.h"																   //Include dxframework ------------------------------------//	
#include "LightShader.h"																													 //
#include "WaterShader.h"																													 //
#include "TerrainMeshExample.h"																												 //
#include "Water.h"																															 //
#include "GenerateName.h"																												     //
#include "DepthShader.h"																													 //
#include "ShadowShader.h"																													 //
#include "terrainGrayScaleShader.h"																											 //
#include "FogShader.h"																														 //
#include "LineMesh.h"																														 //
#include "LSystem.h"																														 //
#include "CreateTree.h"																														 //
#include "CPerlinNoise.h"																													 //
#include "TextureShader.h"																													 //
#include "CAShader.h"																														 //
#include "HorizontalBlur.h"																													 //
#include "VerticalBlur.h"																													 //
#include "Bloom.h"																															 //
//-------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------App Class Inheriting from Base Application------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//	
class App1 : public BaseApplication																											 //
{																																			 //
public:																																		 //
	App1();																																	 //
	~App1();																																 //
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);					 //
	bool frame();																															 //
																																			 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //------------------------------------------------------------Protected Functions------------------------------------------------------------//
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//	
protected:																																   	 //
	bool render();																							//Function to render scene ------//
																											//
																											//------------------------------------------------------------------Setters------------------------------------------------------------------//
	inline void setDetail(int Detail) { DetailLvl = Detail; }																				 //
	inline void setP(int p) { Pheight = p; }																								 //
	inline void setTexture(int h) { Hheight = h; }																							 //
	inline void setFault(int f) { fault = f; }																								 //
	inline void setSmooth(int s) { smooth = s; }																							 //
	inline void setFrequency(float f) { frequency = f; }																					 //
	inline void setHeight(float h) { height = h; }																							 //								
																																			 //------------------------------------------------------------------Getters------------------------------------------------------------------//
	inline float getHeight() { return height; }																								 //
	inline float getFrequency() { return frequency; }																						 //
	inline float getSmooth() { return smooth; }																								 //
	inline float getFault() { return fault; }																								 //
	inline int getTexture() { return Hheight; }																								 //
	inline float getP() { return Pheight; }																									 //
	inline float getDetail() { return DetailLvl; }																							 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//	
	void firstPass();																														 //
	void secondPass();																														 //
	void finalPass();																														 //
	void horizontal();																														 //
	void vertical();																														 //
	void Chromatic();																														 //
	void gui();																							//Function for render ImGui ---------//
																										//-------------------------------------------------------------------------------------------------------------------------------------------//	
																										//-------------------------------------------------------------------------------------------------------------------------------------------//	
																										//
																										//-------------------------------------------------------------------------------------------------------------------------------------------//
																										//-------------------------------------------------------------Private Functions-------------------------------------------------------------//
																										//-------------------------------------------------------------------------------------------------------------------------------------------//																															 
private:																																	 //
																																			 //------------------------------------------------------------------Objects------------------------------------------------------------------//
	LightShader * terrainShader;																	//Terrain shader ----------------------------//
	LightShader* treeShader;																												 //
	WaterShader* waterShader;																	//Eater shader ------------------------------//
	TerrainMeshExample* m_Plane;																//Terrain mesh ------------------------------//
	Water* m_water;																				//Water Plane -------------------------------//
	Light* light; Light* light2; Light* pointLight;												//Scene lights ------------------------------//
	PlaneMesh* waterMesh;																		//Water Plane -------------------------------//
	RenderTexture* renderTexture;																//Render to Texture -------------------------//
	RenderTexture* renderTexture2;																											 //
	RenderTexture* greyTexture;																												 //
	RenderTexture* bloomTexture;																											 //
	GenerateName genName;																		//Generate Name -----------------------------//
	DepthShader* depthShader;																	//Depth shader for shadows ------------------//
	DepthShader* depthShader2;																												 //
	ShadowMap* shadowMap;																													 //
	ShadowShader* shadowShader;																												 //
	CubeMesh* lightPos;																														 //
	SphereMesh* skyBox;																														 //
	terrainGrayShader* terrainGrey;																											 //
	FogShader* fogShader;																													 //
	LineMesh* m_Line;																														 //
	LSystem lSystem;																														 //																								 
	TextureShader* textureShader;																										     //
	OrthoMesh* orthoMesh;																													 //
	CA* CAshader;																															 //
	float time = 0;																															 //
	HorizontalBlurShader* HB;																												 //
	VerticalBlurShader* VB;																													 //
	bloom* Bloom;																															 //
	XMFLOAT3 rgb = XMFLOAT3(0.f, 0.f, 0.f), CAfrequency = XMFLOAT3(0.f, 0.f, 0.f);																 //
	bool blur = false, blur2 = false, ca = false, bloomB = false;																			 //
	RenderTexture* horizontalBlurTexture;												//Render to textures for post processing effects ----//
	RenderTexture* verticalBlurTexture;																										 //
	RenderTexture* CATexture;																												 //
	RenderTexture* shadowTexture;																											 //
	RenderTexture* current;																													 //
																																			 //
																																			 //
																																			 //-------------------------------------------------------------Terrain Variables-------------------------------------------------------------//
	int startH = 0, height = startH, smooth = 0, startS = 0, fault = 1, startFault = 0,		//Int variables for editing the terrain ---------//
		res = 149, botRX = res, botRY = res, topLX = 0, topLY = 0, depth = 1, Hheight,														 //
		startDetail = 0, DetailLvl = startDetail, minDetail = 0, maxDetail = 100,															 //
		feet, currentHeight, terrainResolution = 15;																						 //
	float startF = 0, frequency = startF, startP = 1, Pheight = startP, startBlend = 0.2f;	//Float variables for editing the terrain -------//
	bool overwrite = false;																	//Boolean for overwriting the texture climate ---//
	float greyBlend, pScale = 0.01, pSize = 10;																								 //
	int octaves = 1;																														 //
	float fbmF = 1;																															 //
	bool absolute = false;																													 //
																																			 //
	float bias = 0.025f;																													 //
																																			 //
																																			 //--------------------------------------------------------------Water Variables--------------------------------------------------------------//
																																			 //
	int waterResolution = 149;																	//Set resolution of Water plane -------------//
	float startWater = 0, delta = 0, Astart = 0.46,												//Variables for water vertex manipulation ---//
		Fstart = 0.166, Sstart = 13.73, alphaStart = 0.65f;																				 //
	float tessellation = 4.f;																												 //
	bool changeColour = false;																												 //
	float constFac = 1.f, linFac = 0.125f, dist = 0.2f, quad = 0.f;																			 //
#																																			 //
																																			 //
																																			 //--------------------------------------------------------------Light Variables--------------------------------------------------------------//
	XMFLOAT3 startPPos, lightDirection = XMFLOAT3(1.f, -0.4f, -0.5f);		//Variables for light Position and light Direction --------------//
	XMFLOAT3 lightPDirection = XMFLOAT3(0.6f, -1.f, -0.7f);																					 //
	XMFLOAT3 startDPos = XMFLOAT3(0.f, 0.f, 0.f);							//variables for location of  light ------------------------------//
	XMFLOAT4 lightDColour = XMFLOAT4(1.f, 1.f, 1.f, 1.f);					//Set initial Colour for directional light ----------------------//
	XMFLOAT4 lightPColour = XMFLOAT4(1.f, 0.f, 0.f, 1.f);																					 //
	float startPos, startPosx, startPosz;									//variables for location of spot light --------------------------//
	bool directional = true;												//Boolean to check if directional light is on -------------------//
	XMFLOAT4 specColour = XMFLOAT4(1.f, 1.f, 1.f, 1.f);																						 //
	float spec = 100.f;																														 //
	bool shadows = false, pointLightToggle = false, greyScale = false;																		 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //
																																			 //---------------------------------------------------------------Fog Variables---------------------------------------------------------------//
	XMFLOAT4 fog = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);								   //Set initial fog colour -----------------------------//
	float fogS = 10, fogE = 100, fogStrength = 0;									   //Set fog start and end positions and the strength ---//
	XMFLOAT4 bg = XMFLOAT4(0.39f, 0.58f, 0.92f, 1.f);								   //Set initial background colour ----------------------//
	XMFLOAT4 currentBg = bg;																												 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //
																																			 //----------------------------------------------------------Miscellaneous Variables----------------------------------------------------------//
																																			 //																					
	XMMATRIX m_reflectionViewMatrix;																			//Stroed reflection matrix --//
	std::string name;																							//Store name from getter ----//
	float camSpeed = 5.f;																													 //
};																																			 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //
#endif																																		 //
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 //-------------------------------------------------------------------End---------------------------------------------------------------------//
																																			 //-------------------------------------------------------------------------------------------------------------------------------------------//