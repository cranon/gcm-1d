#include "Body.h"
#include <iostream>
#include <fstream>

using namespace std;

Body::Body() {
	cout << "empty constructor of body" << endl;
}

void Body::setParameters(const char * fileName) {
	cout << "Setting parameters of body" << endl;
	Parser parser;
	parser.Reading(fileName);
	mesh.setParameters(parser.getInitValues(), parser.getNumX());
	rheology = parser.getRheology();
	RightCnrCond = parser.getRight();
	LeftCnrCond = parser.getLeft();
}

void Body::doNextStep(float tau, int methodType, \
						const char * _LeftCnrCond, const char * _RightCnrCond) {
	if (_LeftCnrCond != "Previous") LeftCnrCond = _LeftCnrCond;
	if (_RightCnrCond != "Previous") RightCnrCond = _RightCnrCond;
	/*
	 * Choosing number of method depends on type of method, 
	 * rheology and corner conditions will be here
	 */
	NumMethod method(tau);
	method.FirstOrder(&mesh);
}

void Body::printData(int fileNumber) {
	sprintf(fileName, "data/Body_1_Step_%i.txt", fileNumber);
	mesh.printData(fileName);
}

Body::~Body() {
	cout << "deleting body" << endl;
}

