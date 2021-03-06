#include "Scene.h"

Scene::Scene() {
	fileNumber = 0;
	cout << "Creating scene" << endl;
}

Scene::~Scene() {
	cout << "deleting scene" << endl;
}

int Scene::doNextStep(double maxTau, int methodType) {
	fileNumber = fileNumber + 1;
	if (NumOfBodies == 1) {
		Monitor monitor0(body1.mesh.NumX, maxTau);
		monitor0.getCourant(&body1.mesh);
		double temp = monitor0.monStruct.max;
		if (methodType == 0) {
			int i = 0;
			if ( fabs(temp - 1) > 1e+1 ) {
				temp = monitor0.monStruct.mean;
				maxTau = maxTau / temp;
				cout << "maxTau = " << maxTau << endl;
			}
			do {
				if (i++ > 10) {
					cout << "Warning! 10 iterations were done on " << fileNumber << " time step!\n";
					break;
				}
				body1._mesh = &(body1.mesh);
				if (body1.doNextStep(maxTau, methodType, "Previous", "Previous") == -1) return -1;
				Monitor monitor(body1._mesh.NumX, maxTau);
				monitor.getCourant(&body1._mesh);
				temp = monitor.monStruct.mean;
				if ( fabs(temp - 1) > 1e+1 ) {
					maxTau = maxTau / temp;
					cout << "maxTau = " << maxTau << endl;
				}
			} while ( fabs(temp - 1) > 1.1e+1 );
			body1.mesh = &(body1._mesh);
			body1.printData(fileNumber);
			return 0;
		}
		
		if(temp > 0.99) {
			maxTau = maxTau * 0.99 / temp;
			cout << "maxTau = " << maxTau << endl;
		}
		do {
			body1._mesh = &(body1.mesh);
			if(body1.doNextStep(maxTau,methodType,"Previous","Previous") == -1) return -1;	
			Monitor monitor(body1._mesh.NumX, maxTau);
			monitor.getCourant(&body1._mesh);
			temp = monitor.monStruct.max;
			if(temp > 0.99) {
				maxTau = maxTau * 0.99 / temp;
				cout << "maxTau = " << maxTau << endl;
			}
		} while(temp > 0.995);
		body1.mesh = &(body1._mesh);
		body1.printData(fileNumber);
		if(fileNumber == 1) cout << body1.t << endl;
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

