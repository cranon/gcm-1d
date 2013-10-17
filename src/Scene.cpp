#include "Scene.h"

Scene::Scene() {
	fileNumber = 0;
	cout << "Creating scene" << endl;
}

Scene::~Scene() {
	cout << "deleting scene" << endl;
}

int Scene::doNextStep(float maxTau, int methodType) {
	fileNumber = fileNumber + 1;
	if (NumOfBodies == 1) {
		Monitor monitor(body1.mesh.NumX, maxTau);
		monitor.getCourant(&body1.mesh);
		cout << "maxTau = " << maxTau << endl;
		if(monitor.monStruct.max > 0.9) maxTau = maxTau * 0.9 / monitor.monStruct.max;
		cout << "maxTau = " << maxTau << endl;
		/*if(monitor.MaxV <= 0.1 || monitor.MaxV >= 1.9) {
			cerr << "Monitor: Courant lies by " << monitor.MaxV << endl;
			return -1;
		}*/
		if(body1.doNextStep(maxTau,methodType,"Previous","Previous") == -1) return -1;		
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
		cout << "Scene: max number of bodies are 2!" << endl;
		exit(-1);
	}
}

