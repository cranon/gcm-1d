#include "NumMethod.h"

NumMethod::NumMethod() {
	
}

NumMethod::NumMethod(float _tau) {
	tau = _tau;
}

NumMethod::~NumMethod() {

}

Node NumMethod::FirstOrder_First(Mesh *mesh) {
	Node tmp = mesh->Values[0];
	float w1, w2;
	float a_next = (mesh->Values[0].getA() + mesh->Values[1].getA())/2;;
	
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	w2 = mesh->Values[0].getRiman(2) + (mesh->Values[1].getRiman(2) - mesh->Values[0].getRiman(2))*(tau*a_next)/(mesh->Values[1].x - mesh->Values[0].x);
	w1 = -w2;
	tmp.eps = 2*w2;	
	return tmp;
}



Node NumMethod::FirstOrder_Last(Mesh *mesh) {
	Node tmp = mesh->Values[mesh->NumX - 1];
	float w1, w2;
	float a_prev = (mesh->Values[mesh->NumX - 1].getA() + mesh->Values[mesh->NumX - 2].getA())/2;
	
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	w1 = mesh->Values[mesh->NumX - 1].getRiman(1) + (mesh->Values[mesh->NumX - 1].getRiman(1) - mesh->Values[mesh->NumX - 2].getRiman(1))*(-tau*a_prev)/(mesh->Values[mesh->NumX - 1].x - mesh->Values[mesh->NumX - 2].x);
	w2 = -w1;
	tmp.eps = 2*w2;
	return tmp;
}

void NumMethod::FirstOrder(Mesh *mesh) {
	Node last = FirstOrder_First(mesh);
	Node tmp;
	float w1, w2;
	float a_next, a_prev;
	int i;
	
		for(i = 1; i < mesh->NumX - 1; i++) {
			a_next = (mesh->Values[i+1].getA() + mesh->Values[i].getA())/2;
			a_prev = (mesh->Values[i-1].getA() + mesh->Values[i].getA())/2;
			tmp = mesh->Values[i];
			tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;
			w1 = mesh->Values[i].getRiman(1) + (mesh->Values[i].getRiman(1) - mesh->Values[i-1].getRiman(1))*(-tau*a_prev)/(mesh->Values[i].x - mesh->Values[i-1].x);
			w2 = mesh->Values[i].getRiman(2) + (mesh->Values[i+1].getRiman(2) - mesh->Values[i].getRiman(2))*(tau*a_next)/(mesh->Values[i+1].x - mesh->Values[i].x);
	
		
			tmp.v = (w1 + w2)*(a_next + a_prev)/2;
			tmp.eps = w2 - w1;
			mesh->Values[i-1] = last;
			last = tmp;
		}
	mesh->Values[mesh->NumX - 1] = FirstOrder_Last(mesh);
	mesh->Values[mesh->NumX - 2] = last;
}

