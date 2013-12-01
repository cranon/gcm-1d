#include "Node.h"

Node::Node() {
}

Node::Node(int _num, double _x, double _v, double _eps, double _rho, double _E) {
    num = _num;
    x = _x;
    v = _v;
    eps = _eps;
    rho = _rho;
    E = _E;
}

Node::~Node() {
}

double Node::getRiman(int i) {
	double a = getA();
	if(i == 1) return -v + a*eps;
	if(i == 2) return v + a*eps;
}

double Node::getRiman(int i, double a) {
	if(i == 1) return -v + a*eps;
	if(i == 2) return v + a*eps;
}

double Node::getA() {
	return sqrt(getE()/rho);
}

double Node::getE() {
	switch((rheology.c_str())[0]) {
		case 'e': // elastic model
			return E;
			break;
		case 'p': // plastic model
			if((rheology.c_str())[7] == '1') {
					double ElcLim = 0.0001;
					double FldBgn = 0.00015;
					double FldEnd = 0.0008;
					double BrcPnt = 0.001;
					double E0 = E;
					double d = 0.05;
					E0 = E0 / (1 + d);
					double E1 = E0 / 3;
					double _eps = fabs(eps);
					if (_eps < ElcLim) return E0 * (1 + d);
					if (_eps < FldBgn) return E0 * (d + erfc((_eps - (ElcLim + FldBgn) / 2)*6 / (FldBgn - ElcLim)) / 2);
					if (_eps < FldEnd) return E0 * d;
					if (_eps < BrcPnt) {
						double a = (BrcPnt + FldEnd) / 2;
						double b = (BrcPnt - FldEnd) / 2;
						return E1 * exp(1 / (1 - b * b / (_eps - a) / (_eps - a))) + E0*d;
					}
					return E0*d;
			} else {
					double _eps = fabs(eps);
					if(_eps >= 0.0001 && _eps < 0.000125) return E/3;
					if(_eps >= 0.000125 && _eps < 0.000150) return E/10;
					if(_eps >= 0.00015) return E/30;
					else return E;
			}
			break;
	}
}

