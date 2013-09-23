#include "Mesh.h"
#include <fstream>
#include <iostream>

using namespace std;

Mesh::Mesh() {
		cout << "empty constructor of mesh" << endl;
		NumX = 0;
}

void Mesh::setParameters(Node *InitValues, int _NumX) {
	cout << "setting par-s of mesh" << endl;
	NumX = _NumX;
	Values = new Node [NumX];
	for (int i = 0; i < NumX; i++) {
		Values[i] = InitValues[i];
	}
}

void Mesh::printData(const char * fileName) {
	ofstream dataFile (fileName, ios::out);
	for (int i = 0; i < NumX; i++) {
		dataFile << Values[i].num << "	" << Values[i].x << "	" \
				<< Values[i].v << "	" << Values[i].eps << "	" << \
				Values[i].rho << "	" << Values[i].E << " " << Values[i].getRiman(1) << " " << Values[i].getRiman(2) << endl;
	}
	dataFile.close();
}

Mesh::~Mesh() {
	cout << "deleting mesh" << endl;
	if (NumX != 0) {
		delete [] Values;
	}
}

