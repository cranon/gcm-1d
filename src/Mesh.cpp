#include "Mesh.h"

Mesh::Mesh() {
		NumX = 0;
		rheology = "NULL";
		cout << "empty constructor of mesh" << endl;
}

Node Mesh::getNode(int i) {
	return Values[i];
}

void Mesh::operator=(Mesh* mesh2) {
	NumX = mesh2->NumX;
	rheology = mesh2->rheology;
	for (int i = 0; i < NumX; i++) {
		Values[i] = mesh2->getNode(i);
	}
}

void Mesh::setParameters(Node *InitValues, int _NumX, string _rheology) {
	cout << "setting par-s of mesh" << endl;
	NumX = _NumX;
	Values = new Node [NumX];
	rheology = _rheology;
	for (int i = 0; i < NumX; i++) {
		Values[i] = InitValues[i];
	}
}

void Mesh::printData(const char * fileName) {
	ofstream dataFile (fileName, ios::out);
	dataFile.precision(15);
	for (int i = 0; i < NumX; i++) {
		dataFile << scientific << Values[i].num << "\t" << Values[i].x << "\t" \
				<< Values[i].v << "\t" << Values[i].eps << "\t" << \
				Values[i].rho << "\t" << Values[i].getE() << "\t" << Values[i].getA() \
				<< "\t" << Values[i].getRiman(1) << endl;
	}
	dataFile.close();
}

void Mesh::printData(const char * fileName, struct MonStruct *monStruct) {
	ofstream dataFile (fileName, ios::out);
	dataFile.precision(15);
	int i;
	for (i = 0; i < monStruct->Num; i++) {
		dataFile << scientific << Values[i].num << "\t" << Values[i].x << "\t" \
				<< Values[i].v << "\t" << Values[i].eps << "\t" << \
				Values[i].rho << "\t" << Values[i].getE() << \
				"\t" << monStruct->monArr[i] << endl;
	}
	for (i = monStruct->Num; i < NumX; i++) {
		dataFile << Values[i].num << "\t" << Values[i].x << "\t" \
				<< Values[i].v << "\t" << Values[i].eps << "\t" << \
				Values[i].rho << "\t" << Values[i].getE() << endl;
	}
	cout << fileName << " max = " << monStruct->max << endl;
	dataFile.close();
}

Mesh::~Mesh() {
	cout << "deleting mesh" << endl;
	if (NumX != 0) {
		delete [] Values;
	}
}

