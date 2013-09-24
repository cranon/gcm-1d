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

float Node::getRiman(int i) {
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
			break;
	}
}

