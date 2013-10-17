#include "Monitor.h"

Monitor::Monitor() {	
}

Monitor::Monitor(int _Num, float _tau) {
	tau = _tau;
	MaxV = 0.0;
	monStruct.Num = _Num;
	monStruct.monArr = new float [monStruct.Num];
}

Monitor::Monitor(int _Num, float _tau, string _rheology) {
	monStruct.Num = _Num;
	monStruct.monArr = new float [monStruct.Num];
	MaxV = 0.0;
	tau = _tau;
	rheology = _rheology;
}

void Monitor::getH(Mesh *mesh) {
	
	monStruct.monArr[0] = mesh->Values[1].x - mesh->Values[0].x;
	monStruct.mean = monStruct.monArr[0];
	monStruct.min = monStruct.mean;
	monStruct.max = monStruct.min;
	
	int i;
	for(i = 1; i < mesh->NumX-1; i++) {
		monStruct.monArr[i] = mesh->Values[i+1].x - mesh->Values[i].x;
		monStruct.mean += monStruct.monArr[i];
		if(monStruct.max < monStruct.monArr[i]) monStruct.max = monStruct.monArr[i];
		if(monStruct.min > monStruct.monArr[i]) monStruct.min = monStruct.monArr[i];
	}
	monStruct.mean = monStruct.mean/(mesh->NumX - 1);
	
	while(i <= monStruct.Num - 1) {
		monStruct.monArr[i++] = 0.0;
	}
}

void Monitor::getCourant(Mesh *mesh) {
	
	monStruct.monArr[0] = mesh->Values[0].getA() * tau / (mesh->Values[1].x - mesh->Values[0].x);
	monStruct.mean = monStruct.monArr[0];
	monStruct.min = monStruct.mean;
	monStruct.max = monStruct.min;
	
	int i;
	float temp;
	for(i = 1; i < mesh->NumX; i++) {
		monStruct.monArr[i] = mesh->Values[i].getA() * tau / fmin(mesh->Values[i].x - mesh->Values[i-1].x, mesh->Values[i+1].x - mesh->Values[i].x);
		monStruct.mean += monStruct.monArr[i];
		if(monStruct.max < monStruct.monArr[i]) monStruct.max = monStruct.monArr[i];
		if(monStruct.min > monStruct.monArr[i]) monStruct.min = monStruct.monArr[i];
	}
	
	monStruct.monArr[mesh->NumX-1] = mesh->Values[mesh->NumX-1].getA() * tau / (mesh->Values[mesh->NumX-1].x - mesh->Values[mesh->NumX-2].x);
	monStruct.mean += monStruct.monArr[0];
	if(monStruct.max < monStruct.monArr[mesh->NumX-1]) monStruct.max = monStruct.monArr[mesh->NumX-1];
	if(monStruct.min > monStruct.monArr[mesh->NumX-1]) monStruct.min = monStruct.monArr[mesh->NumX-1];
	
	monStruct.mean = monStruct.mean/(mesh->NumX);
	if(monStruct.max > MaxV) MaxV = monStruct.max;
}

Monitor::~Monitor() {
	if (monStruct.Num != 0) {
		delete [] monStruct.monArr;
	}
}
