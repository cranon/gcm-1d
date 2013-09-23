#include "Node.h"
#include <cmath>

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
	return sqrt(E/rho);
}

