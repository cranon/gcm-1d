#include "Node.h"

Node::Node() {
}

Node::Node(int _num, float _x, float _v, float _eps, float _rho, float _E) {
    num = _num;
    x = _x;
    v = _v;
    eps = _eps;
    rho = _rho;
    E = _E;
}

Node::~Node() {
}

float Node::getInv(int i, float A) {
	if (i == 1) return (A*v - eps);
	if (i == 2) return (A*v + eps);
}

float Node::getRiman(int i) {
	// Return Riman's invariant in this node
	// For First Order method
	float a = getA();
	if(i == 1) return (v - a*eps)/2/a;
	if(i == 2) return (v + a*eps)/2/a;
}

float Node::getA() {
	switch((rheology.c_str())[0]) {
		case 'e': // elastic model
			return sqrt(E/rho);
			break;
		case 'p': // plastic model
			return sqrt(getE(eps)/rho);
					
			/*
			if(fabs(eps) >= 0.0001 && fabs(eps) < 0.000125) {
				return sqrt(E/3/rho);
			}
			if(fabs(eps) >= 0.000125 && fabs(eps) < 0.000150) {
				return sqrt(E/10/rho);
			}
			if(fabs(eps) >= 0.00015) {
				return sqrt(E/30/rho);
			}
			else return sqrt(E/rho);
			break;*/
	}
}

float Node::getE(float _eps) {
	float ElcLim = 0.002;
	float FldBgn = 0.003;
	float FldEnd = 0.006;
	float BrcPnt = 0.01;
	float E0 = E;
	float d = 0.1;
	E0 = E0 / (1 + d);
	float E1 = E0 / 3;
	eps = fabs(eps);
	if (eps < ElcLim) return E0 * (1 + d);
	if (eps < FldBgn) return E0 * (d + erfc((eps - (ElcLim + FldBgn) / 2)*6 / (FldBgn - ElcLim)) / 2);
	if (eps < FldEnd) return E0 * d;
	if (eps < BrcPnt) {
		float a = (BrcPnt + FldEnd) / 2;
		float b = (BrcPnt - FldEnd) / 2;
		return E1 * exp(1 / (1 - b * b / (eps - a) / (eps - a))) + E0*d;
	}
//	cout << "The Node is destroyed\n";
	return E0*d;
}
