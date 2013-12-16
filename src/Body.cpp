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
	tmpMesh.setParameters(parser.getInitValues(), parser.getNumX(), rheology);
	Left = parser.getCnrCondition(true);
	Right = parser.getCnrCondition(false);
}

int Body::doNextStep(double tau, int methodType, \
						const char * _LeftCnrCond, const char * _RightCnrCond) {
	NumMethod method(&_mesh, &tmpMesh, tau);
	if (methodType == 0) {
		double Lval = Left.val1;
		bool LisEps = Left.isEps1;
		if (t > Left.t1) {
			Lval = 0.0;
			LisEps = true;
		}
		double Rval = Right.val1;
		bool RisEps = Right.isEps1;
		if (t > Right.t1) {
			Rval = 0.0;
			RisEps = true;
		}
		t = t + tau;
		return method.ImplicitSecondOrder(LisEps, Lval, RisEps, Rval);
	}
	
	Node first, last;
	double val = Left.val1;
	bool isEps = Left.isEps1;
	if (t > Left.t1) {
		val = 0.0;
		isEps = true;
	}
	switch (methodType) {
		case 1 : first = method.FirstOrder_First(isEps, val); break;
		case 2 : first = method.SecondOrder_First(isEps, val); break;
	}
	val = Right.val1;
	isEps = Right.isEps1;
	if (t > Right.t1) {
		val = 0.0;
		isEps = true;
	}
	switch (methodType) {
		case 1 : last = method.FirstOrder_Last(isEps, val); break;
		case 2 : last = method.SecondOrder_Last(isEps, val); break;
	}
	switch (methodType) {
		case 1: if(method.FirstOrder(first, last) == -1) return -1; break;
		case 2: if(method.TimeSecondOrder(first, last) == -1) return -1; break;
	}
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

void Body::printRheology() {
	ofstream dataFile ("Rheology", ios::out);
	dataFile.precision(15);
	Node smpl;
	double maxEps = 15e-4;
	int N = 200;
	for (int i = 0; i < N; i++) {
		smpl.eps = maxEps/N*i;
		dataFile << scientific << smpl.eps << "\t" << smpl.getE() << endl;
	}
	dataFile.close();
}