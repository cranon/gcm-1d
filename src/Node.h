#ifndef NODE_H
#define	NODE_H
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class Node {
public:
    int num;
    double x,v,eps,rho,E;
	string rheology;
    Node();
    Node(int _num, double _x, double _v, double _eps, double _rho, double _E);
    double getRiman(int i);
    double getRiman(int i, double a);
	double getA();
	double getE();
	~Node();
};

#endif	/* NODE_H */

