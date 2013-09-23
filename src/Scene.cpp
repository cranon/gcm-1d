#include "Scene.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>

using namespace std;

Scene::Scene() {
	fileNumber = 0;
	cout << "Creating scene" << endl;
}

Scene::~Scene() {
	cout << "deleting scene" << endl;
}

void Scene::doNextStep(float maxTau, int methodType) {
	fileNumber = fileNumber + 1;
	if (NumOfBodies == 1) {
		/*
		 * Setting maxTau depends on condition of the stability will be here 
		 */
		body1.doNextStep(maxTau,methodType,"Previous","Previous");
		body1.printData(fileNumber);
	}
}

void Scene::Init(int _NumOfBodies, const char * _ContCond, bool _inContact) {
	NumOfBodies = _NumOfBodies;
	if (NumOfBodies == 1) {
		inContact = false;
		ContCond = NULL;
		body1.setParameters("body1.txt");
		body1.printData(0);
	}
	else if (NumOfBodies == 2) {
		inContact = _inContact;
		ContCond = _ContCond;
		body1.setParameters("body1.txt");
		body1.printData(0);
		body2.setParameters("body2.txt");
		body2.printData(0);
	}
	else {
		cout << "Max number of bodies are 2!" << endl;
		exit(-1);
	}
}

