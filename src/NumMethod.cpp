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
	float a = sqrt(mesh->Values[0].E/mesh->Values[0].rho);
	
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	w2 = ((mesh->Values[1].x - mesh->Values[0].x - a*tau)*mesh->Values[0].getRiman(2) + a*tau*mesh->Values[1].getRiman(2))/(mesh->Values[1].x - mesh->Values[0].x);
	w1 = -w2;
	tmp.eps = 2*w2;	
	return tmp;
}



Node NumMethod::FirstOrder_Last(Mesh *mesh) {
	Node tmp = mesh->Values[mesh->NumX - 1];
	float w1, w2;
	float a = sqrt(mesh->Values[mesh->NumX - 1].E/mesh->Values[mesh->NumX - 1].rho);
	
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	w1 = ((mesh->Values[mesh->NumX - 1].x - mesh->Values[mesh->NumX - 2].x - a*tau)*mesh->Values[mesh->NumX - 1].getRiman(1) + a*tau*mesh->Values[mesh->NumX - 2].getRiman(1))/(mesh->Values[mesh->NumX - 1].x - mesh->Values[mesh->NumX - 2].x);
	w2 = -w1;
	tmp.eps = 2*w2;
	return tmp;
}

void NumMethod::FirstOrder(Mesh *mesh) {
	Node last = FirstOrder_First(mesh);
	Node tmp;
	float w1, w2;
	float a, a_next, a_prev, a_near;
	int i;
	
		for(i = 1; i < mesh->NumX - 1; i++) {
			a = sqrt(mesh->Values[i].E/mesh->Values[i].rho);
			a_next = sqrt(mesh->Values[i+1].E/mesh->Values[i+1].rho);
			a_prev = sqrt(mesh->Values[i-1].E/mesh->Values[i-1].rho);
			tmp = mesh->Values[i];
			tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;
			w1 = mesh->Values[i].getRiman(1) + (mesh->Values[i].getRiman(1) - mesh->Values[i-1].getRiman(1))*(-tau*(a+a_prev)/2)/(mesh->Values[i].x - mesh->Values[i-1].x);
			w2 = mesh->Values[i].getRiman(2) + (mesh->Values[i+1].getRiman(2) - mesh->Values[i].getRiman(2))*(tau*(a+a_next)/2)/(mesh->Values[i+1].x - mesh->Values[i].x);
			a_near = a_next;
			if (fabs(w2) > fabs(w1)) a_near = a_prev;
			tmp.v = (w1 + w2)*(a + a_near)/2;
			tmp.eps = w2 - w1;
			mesh->Values[i-1] = last;
			last = tmp;
		}
	mesh->Values[mesh->NumX - 1] = FirstOrder_Last(mesh);
	mesh->Values[mesh->NumX - 2] = last;
}

