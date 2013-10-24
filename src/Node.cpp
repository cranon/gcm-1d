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
	float a = getA();
	if(i == 1) return (v - a*eps)/a/2;
	if(i == 2) return (v + a*eps)/a/2;
}

float Node::getA() {
	return sqrt(getE()/rho);
}

float Node::getE() {
	switch((rheology.c_str())[0]) {
		case 'e': // elastic model
			return E;
			break;
		case 'p': // plastic model
			float ElcLim = 0.002;
			float FldBgn = 0.003;
			float FldEnd = 0.006;
			float BrcPnt = 0.01;
			float E0 = E;
			float d = 0.1;
			E0 = E0 / (1 + d);
			float E1 = E0 / 3;
			float _eps = fabs(eps);
			if (_eps < ElcLim) return E0 * (1 + d);
			if (_eps < FldBgn) return E0 * (d + erfc((_eps - (ElcLim + FldBgn) / 2)*6 / (FldBgn - ElcLim)) / 2);
			if (_eps < FldEnd) return E0 * d;
			if (_eps < BrcPnt) {
				float a = (BrcPnt + FldEnd) / 2;
				float b = (BrcPnt - FldEnd) / 2;
				return E1 * exp(1 / (1 - b * b / (_eps - a) / (_eps - a))) + E0*d;
			}
			//	cout << "The Node is destroyed\n";
			return E0*d;
/*
			if(_eps >= 0.0001 && _eps < 0.000125) return E/3;
			if(_eps >= 0.000125 && _eps < 0.000150) return E/10;
			if(_eps >= 0.00015) return E/30;
			else return E;
			break;
	*/
	}
}


