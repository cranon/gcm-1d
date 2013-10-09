#include "NumMethod.h"

using namespace std;

NumMethod::NumMethod() {
	
}

NumMethod::NumMethod(Mesh* _mesh, float _tau) {
	mesh = _mesh;
	tau = _tau;
	limitor = true;
	proxima = new Approximator(_mesh, limitor);
}

NumMethod::~NumMethod() {

}

// Left Approximation 'a' in 'num' node by 'order' order
float NumMethod::lAppA(int num, int order) {
	switch(order) {
		case 0:
			return mesh->Values[num].getA();
		case 1:
			return this->lAppA(num, 0)*
				(tau*(this->lAppA(num-1, 0) - this->lAppA(num, 0)) + mesh->Values[num].x - mesh->Values[num-1].x)/ \
				(mesh->Values[num].x - mesh->Values[num-1].x);
		case 2:
			if(num == 1) {
				vec b;
				b << this->lAppA(0, 0) << this->lAppA(1, 0) << this->lAppA(2, 0);
				return proxima->QuadraticAppr(0, &b, mesh->Values[1].x - tau*lAppA(1, 1));		
			}
			vec b;
			b << this->lAppA(num-2, 0) << this->lAppA(num-1, 0) << this->lAppA(num, 0);
			return proxima->QuadraticAppr(num-2, &b, mesh->Values[num].x - tau*lAppA(num, 1));
	}
}

// Right Approximation 'a' in 'num' node by 'order' order
float NumMethod::rAppA(int num, int order) {
	switch(order) {
		case 0:
			return mesh->Values[num].getA();
		case 1:
			return this->rAppA(num, 0)* \
				(tau*(this->rAppA(num+1, 0) - this->rAppA(num, 0)) + mesh->Values[num+1].x - mesh->Values[num].x)/ \
				(mesh->Values[num+1].x - mesh->Values[num].x);
		case 2:
			if(num == mesh->NumX - 2) {
				vec b;
				b << this->rAppA(num-1, 0) << this->rAppA(num, 0) << this->rAppA(num+1, 0);
				return proxima->QuadraticAppr(num-1, &b, mesh->Values[num].x + tau*rAppA(num, 1));		
			}
			vec b;
			b << this->rAppA(num, 0) << this->rAppA(num+1, 0) << this->rAppA(num+2, 0);
			return proxima->QuadraticAppr(num, &b, mesh->Values[num].x + tau*rAppA(num, 1));
	}
}

// First order for a first node
Node NumMethod::FirstOrder_First() { 
	Node tmp = mesh->Values[0];
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	tmp.eps = 2*proxima->LinearAppr(0, mesh->Values[0].getRiman(2), mesh->Values[1].getRiman(2), tau*rAppA(0, 1) - mesh->Values[0].x);
	return tmp;
}

// Second order for a first node
Node NumMethod::SecondOrder_First() { 
	Node tmp = mesh->Values[0];
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	vec b;
	b << mesh->Values[0].getRiman(2) << mesh->Values[1].getRiman(2) << mesh->Values[2].getRiman(2);
	tmp.eps = 2*proxima->QuadraticAppr(0, &b, mesh->Values[0].x + tau*rAppA(0, 2));	
	return tmp;
}

// First order for a last node
Node NumMethod::FirstOrder_Last() {
	Node tmp = mesh->Values[mesh->NumX - 1];
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	tmp.eps = -2*proxima->LinearAppr(mesh->NumX-2, mesh->Values[mesh->NumX-2].getRiman(1), mesh->Values[mesh->NumX-1].getRiman(1), mesh->Values[mesh->NumX-1].x - tau*lAppA(mesh->NumX-1, 1));
	return tmp;
}

// Second order for a last node
Node NumMethod::SecondOrder_Last() {
	Node tmp = mesh->Values[mesh->NumX - 1];
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	vec b;	
	b << mesh->Values[mesh->NumX-3].getRiman(1) << mesh->Values[mesh->NumX-2].getRiman(1) << mesh->Values[mesh->NumX-1].getRiman(1);
	tmp.eps = -2*proxima->QuadraticAppr(mesh->NumX-3, &b, mesh->Values[mesh->NumX-1].x - tau*lAppA(mesh->NumX-1, 2));
	return tmp;
}

// First order
int NumMethod::FirstOrder() {
	Node last = FirstOrder_First();
	Node tmp;
	float w1, w2;
	int i;
		for(i = 1; i < mesh->NumX - 1; i++) {
			tmp = mesh->Values[i];
			tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;
			
			w1 = proxima->LinearAppr(i-1, mesh->Values[i-1].getRiman(1), mesh->Values[i].getRiman(1), mesh->Values[i].x - tau*lAppA(i, 1));
			w2 = proxima->LinearAppr(i, mesh->Values[i].getRiman(2), mesh->Values[i+1].getRiman(2), mesh->Values[i].x + tau*rAppA(i, 1));
			
			tmp.v = (w1 + w2)*mesh->Values[i].getA();
			
			// To indicate large displacement
			if(fabs(tmp.x - mesh->Values[i].x) > fmax(fabs(mesh->Values[i].x - mesh->Values[i-1].x), fabs(mesh->Values[i].x - mesh->Values[i+1].x))/3) { cout << "PIZDEC' X i="<< i << endl; return -1; } 
			tmp.eps = w2 - w1;
			
			mesh->Values[i-1] = last;
			last = tmp;
		}
	mesh->Values[mesh->NumX - 1] = FirstOrder_Last();
	mesh->Values[mesh->NumX - 2] = last;
}

int NumMethod::SecondOrder() {
	Node tmp;
	
	// 'temp1' is the first node
	Node temp1 = SecondOrder_First(); 	
	
	// 'temp2' is the second node with distorted 'w1' pattern
	Node temp2 = mesh->Values[1];
	temp2.x = mesh->Values[1].x + tau*mesh->Values[1].v;
	
	vec b;
	b << mesh->Values[0].getRiman(1) << mesh->Values[1].getRiman(1) << mesh->Values[2].getRiman(1);
	float w1 = proxima->QuadraticAppr(0, &b, mesh->Values[1].x - tau*lAppA(1, 2));

	b << mesh->Values[1].getRiman(2) << mesh->Values[2].getRiman(2) << mesh->Values[3].getRiman(2);
	float w2 = proxima->QuadraticAppr(1, &b, mesh->Values[1].x + tau*rAppA(1, 2));
	
	temp2.v = (w1 + w2)*mesh->Values[1].getA();
	temp2.eps = w2 - w1;
	
	// General calculation with standart pattern
	int i;
	for(i = 2; i < mesh->NumX - 2 ; i++) {
		tmp = mesh->Values[i];
		tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;

		b << mesh->Values[i-2].getRiman(1) << mesh->Values[i-1].getRiman(1) << mesh->Values[i].getRiman(1);
		w1 = proxima->QuadraticAppr(i-2, &b, mesh->Values[i].x - tau*lAppA(i, 2));
		
		b << mesh->Values[i].getRiman(2) << mesh->Values[i+1].getRiman(2) << mesh->Values[i+2].getRiman(2);
		w2 = proxima->QuadraticAppr(i, &b, mesh->Values[i].x + tau*rAppA(i, 2));
		
		tmp.v = (w1 + w2)*mesh->Values[i].getA();
		tmp.eps = w2 - w1;
		
		mesh->Values[i-2] = temp1;
		temp1 = temp2;
		temp2 = tmp;
	}
	
	// 'tmp' is the last but one node with distorted 'w2' pattern
	tmp = mesh->Values[i];
	tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;

	b << mesh->Values[i-2].getRiman(1) << mesh->Values[i-1].getRiman(1) << mesh->Values[i].getRiman(1);
	w1 = proxima->QuadraticAppr(i-2, &b, mesh->Values[i].x - tau*lAppA(i, 2));
	
	b << mesh->Values[i-1].getRiman(2) << mesh->Values[i].getRiman(2) << mesh->Values[i+1].getRiman(2);
	w2 = proxima->QuadraticAppr(i-1, &b, mesh->Values[i].x + tau*rAppA(i, 2));
	
	tmp.v = (w1 + w2)*mesh->Values[i].getA();
	tmp.eps = w2 - w1;
	
	mesh->Values[i-2] = temp1;
	
	// The last node
	mesh->Values[i+1] = SecondOrder_Last();
	mesh->Values[i-1] = temp2;
	mesh->Values[i] = tmp;
}

