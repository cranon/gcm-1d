#ifndef NODE_H
#define	NODE_H

class Node {
public:
    int num;
    float x,v,eps,rho,E;
    Node();
    Node(int _num, float _x, float _v, float _eps, float _rho, float _E);
    float getRiman(int i);
	~Node();
};

#endif	/* NODE_H */

