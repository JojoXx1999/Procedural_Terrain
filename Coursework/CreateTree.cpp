#include "CreateTree.h"

CreateTree::CreateTree() : lSystem("A")
{

}

CreateTree::~CreateTree()
{

}

void CreateTree::addRules() {
	lSystem.AddRule('A', "B[A]A");
	lSystem.AddRule('B', "BB");
	lSystem.Run(3);
}

void CreateTree::Render(int i, LineMesh* m_Line) {

	m_Line->sendData(renderer->getDeviceContext(), i);

}

void CreateTree::BuildLine(LineMesh* m_Line) {
	//Clear any lines we might already have
	m_Line->Clear();

	//Get the current L-System string
	std::string systemString = lSystem.GetCurrentSystem();

	//Initialise some variables
	XMFLOAT3 p(0, 0, 0);
	XMVECTOR pos = XMLoadFloat3(&p);	//Current position
	XMVECTOR currentPos = pos;

	p = XMFLOAT3(0, 1, 0);
	XMVECTOR dir = XMLoadFloat3(&p);	//Current direction
	XMVECTOR currentDir = dir;

	p = XMFLOAT3(0, 0, 1);
	XMVECTOR fwd = XMLoadFloat3(&p);	//Rotation axis

										//Define a matrix for rotating our direction		
	XMMATRIX rot = XMMatrixRotationAxis(fwd, AI_DEG_TO_RAD(45.0f));
	XMMATRIX rot2 = XMMatrixRotationAxis(fwd, AI_DEG_TO_RAD(-45.0f));

	//Start and end of each line segment
	XMFLOAT3 start, end;
	vector <XMVECTOR>list, list2;
	//Go through the L-System string
	for (int i = 0; i < systemString.length(); i++) {
		switch (systemString[i]) {

			//If it's an 'A', then make a line
		case 'A':
			XMStoreFloat3(&start, pos);			//Store the start position
			pos += dir;							//Move the position marker
			XMStoreFloat3(&end, pos);			//Store the end position
			m_Line->AddLine(start, end);		//Create the line
			break;

		case 'B':
			XMStoreFloat3(&start, pos);			//Store the start position
			pos += dir;							//Move the position marker
			XMStoreFloat3(&end, pos);			//Store the end position
			m_Line->AddLine(start, end);		//Create the line
			break;
			//If it's a 'B', then make a rotate the direction of the next line
		case '[':
			list.push_back(pos);
			list2.push_back(dir);
			dir = XMVector3Transform(dir, rot2);	//Rotate the direction 45 degrees

			break;

		case ']':
			pos = list.back();
			list.pop_back();
			dir = list2.back();
			list2.pop_back();
			dir = XMVector3Transform(dir, rot);	//Rotate the direction 45 degrees

			break;

		}
	}
}