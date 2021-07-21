//-------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------By Jodie Duff----------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------------------------//
#include "GenerateName.h"																													 
//-------------------------------------------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------Init Generate Name------------------------------------------------------------//
GenerateName::GenerateName() {																												 
	srand(time(NULL));//base random number on time ----------------//
	MakeName();		//Call Make Name function -------------------//
}																																			 
//-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 
//---------------------------------------------------------Generate Name Destructor----------------------------------------------------------//
GenerateName::~GenerateName() {}																									         
//-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 
//-----------------------------------------------------------------Reset Name----------------------------------------------------------------//
void GenerateName::Reset() {	//Reset Variables -----------------------------------																											 //
	check = false; addedDes2 = false; addedDes = false;																					     
	currentDes = "";																														 
	name.erase(0, name.length());																										     
	des = 0;																																 
	size = 0;																																 
}																																			 
//-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 
//----------------------------------------------------------------Change Name----------------------------------------------------------------//
void GenerateName::ChangeName(float climate, int ft) {																						 
	MakeName();																																 
	AddDes(climate);																														 
	AddGeo(ft);																																 
}																																			 
//-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 
//--------------------------------------------------------------Create Base Name-------------------------------------------------------------//
void GenerateName::MakeName() {																												 
	name = "";	//Clear name --------------------//
	int pre = rand() % (sizeof(prefix) / sizeof(prefix[0]));	//Random number between 0 and length of the prefix array 
	int suf = rand() % (sizeof(suffix) / sizeof(suffix[0]));	//Random number between 0 and length of the suffix array 
	int mid = rand() % (sizeof(middle) / sizeof(middle[0]));	//Random number between 0 and length of the middle array 
																				
	name = prefix[pre];		//Set name to the chosen prefix 
	int addMid = rand() % 5 + 1;	//Generate random number between 1 and 5
	if (addMid == 1) name.append(middle[mid]);	//If the generated number is equal to 1 Add the chosen middle
	name.append(suffix[suf]);	//Add on the chosen suffix																													
}																																			 
 //-------------------------------------------------------------------------------------------------------------------------------------------//
																																			 
 //---------------------------------------------------------------Add descriptor--------------------------------------------------------------//
void GenerateName::AddDes(float climate) {																									 
																																			 
	if (climate >= 0.7 && addedDes == false) {	//If terrain climate is greater or equal to 0.7 
		addedDes = true;//test so that only one descriptor can be added 
		addedDes2 = false;																													 
		des = rand() % (sizeof(coldDescriptor) / sizeof(coldDescriptor[0]));   //Get the descriptor text
		name.erase(0, size);	 //Erase any previous descriptor 
		size = coldDescriptor[des].length();//Store the size of the new descriptor 
		name.insert(0, coldDescriptor[des]);//Insert the new descritor at the start of the name 
	}																																		 
	else if (climate < 0.7)	 //If climate is less than 0.7 
	{																																		 
		addedDes2 = true;																													 
		addedDes = false;																													 																						
		name.erase(0, size);																												 
		if (currentDes == "") {		//If there is current not descriptor 
			des = rand() % (sizeof(descriptor) / sizeof(descriptor[0]));//Generate a descriptor 
			size = descriptor[des].length();	//Get size of the descriptor 
			name.insert(0, descriptor[des]);	//Add the descriptor to the start of the name
			currentDes = descriptor[des];		//set current Descritor to the descriptor 
		}																																	 
		else    //If there is a descriptor 
		{																																	 
			name.insert(0, currentDes);		//Add the descriptor to the start of the name
			size = currentDes.length();		//set the size to descriptor length 
		}																																	 
	}																																		 
}																																			 
 //------------------------------------------------------------Add Geometry to Name-----------------------------------------------------------//
void GenerateName::AddGeo(int ft) {																											 
	if (check == false) {	//Only add one word to the name
		check = true;																														 
		int create = rand() % 2 + 1; //Generate a number between 1 and 2 
		if (create == 1) {	//If this number is 1 
			if (ft < 2000) {	//If the highest point of terrain is less than 2000ft 										
				if (ft >= 800) {													//If it is higher than 800 feet pick a name from geometry
					int geo = rand() % (sizeof(geometry) / sizeof(geometry[0]));														     
					name.append(geometry[geo]);									    //Add geometry onto the end of the name 
				}																															 
				else                                                                //If highest point is less than 800 feet 
				{																															 
					int geo = rand() % (sizeof(geometry) / sizeof(geometry[0]) - 1); //Choose a name from geometry except from 'Hill'
					name.append(geometry[geo]);																								 
				}																															 
			}																																 
			else if (ft >= 2000 && ft < 3000) {										 //If highest point is between 2000 and 3000 feet 
				int geo = rand() % (sizeof(highGeometry) / sizeof(highGeometry[0]) - 1); //Choose from highGeometry except 'Monro'										
																						 
				name.append(highGeometry[geo]);																							 
			}																																 
			else {																		//If highest point is 3000 or above 
				int geo = rand() % (sizeof(highGeometry) / sizeof(highGeometry[0]));	//Pick any word from highGeometry 
				name.append(highGeometry[geo]);																							     
			}																																 
		}																																	 
	}																																	     
}																																			 
//-------------------------------------------------------------------------------------------------------------------------------------------//