#include "Body.h"

Body::Body() {
	cout << "empty constructor of body" << endl;
}

void Body::setParameters(const char * fileName) {
	cout << "Setting parameters of body" << endl;
	t = 0;
	Parser parser;
	parser.Reading(fileName);
	rheology = parser.getRheology();
	mesh.setParameters(parser.getInitValues(), parser.getNumX(), rheology);
	_mesh.setParameters(parser.getInitValues(), parser.getNumX(), rheology);
	Left = parser.getCnrCondition(true);
	Right = parser.getCnrCondition(false);
}

int Body::doNextStep(float tau, int methodType, \
						const char * _LeftCnrCond, const char * _RightCnrCond) {
	/*
	 * Choosing number of method and setting extForce depends on type of method, 
	 * rheology and corner conditions will be here
	 */
	NumMethod method(&_mesh, tau);
	
	float val = Left.val1;
	bool isEps = Left.isEps1;
	if (t > Left.t1) {
		val = 0.0;
		isEps = true;
	}
	Node first = method.SecondOrder_First(isEps, val);
	
	val = Right.val1;
	isEps = Right.isEps1;
	if (t > Right.t1) {
		val = 0.0;
		isEps = true;
	}
	Node last = method.SecondOrder_Last(isEps, val);
	if(method.SecondOrder(first, last) == -1) return -1;
	t = t + tau;
}

void Body::printData(int fileNumber) {
	sprintf(fileName, "data/Body_1_Step_%i.txt", fileNumber);
	mesh.printData(fileName);
}

void Body::printData(int fileNumber, struct MonStruct *monStruct) {
	sprintf(fileName, "data/Body_1_Step_%i.txt", fileNumber);
	mesh.printData(fileName, monStruct);
}

Body::~Body() {
	cout << "deleting body" << endl;
}

