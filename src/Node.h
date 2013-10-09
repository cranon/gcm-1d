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
    float x,v,eps,rho,E;
	string rheology;
    Node();
    Node(int _num, float _x, float _v, float _eps, float _rho, float _E);
    float getRiman(int i);
	float getInv(int i, float A);
	float getA();
	~Node();
};

#endif	/* NODE_H */

