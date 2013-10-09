#include "NumMethod.h"
#include "cstdio"
#include <armadillo>

using namespace std;
using namespace arma;

NumMethod::NumMethod() {
}

NumMethod::NumMethod(float _tau) {
	tau = _tau;
}

NumMethod::~NumMethod() {
}

int NumMethod::ImplicitSecondOrder(Mesh *mesh) {
	int N = mesh->NumX;
	float a[N], b[N], c[N], d[N], e[N], f[N], P[N], Q[N], M[N], L[N], r1[N], r2[N];
	for (int k = 1; k < N; k++) {
		float A = 0.5*(mesh->Values[k].getA() + mesh->Values[k-1].getA());
		float g1 = 0; // fix normal calculating g!
		float g2 = 0; // fix normal calculating g!
		float h = mesh->Values[k].x - mesh->Values[k-1].x;
		a[k] = 1 - A*tau/h;
		b[k] = 1 + A*tau/h;
		c[k] = 2*A*tau*g1 - 2*tau*g2 + a[k]*mesh->Values[k].getInv(1,A) + \
				b[k]*mesh->Values[k-1].getInv(1,A);
	}
	for (int k = 0; k < N-1; k++) {
		float A = 0.5*(mesh->Values[k].getA() + mesh->Values[k+1].getA());
		float g1 = 0; // fix normal calculating g!
		float g2 = 0; // fix normal calculating g!
		float h = mesh->Values[k+1].x - mesh->Values[k].x;
		e[k] = 1 - A*tau/h;
		d[k] = 1 + A*tau/h;
		f[k] = 2*A*tau*g1 + 2*tau*g2 + e[k]*mesh->Values[k].getInv(2,A) + \
				d[k]*mesh->Values[k+1].getInv(2,A);
	}
	
	double eta = 0;	// fix normal choosing eta, ...
	double dzeta = 0;
	double ksi = 1;
	double kapa = 1;
	
	P[1] = -a[1]/b[1];
	Q[1] = c[1]/b[1];
	for (int i = 2; i < N; i++) {
		P[i] = -P[i-1]*a[i]/b[i];
		Q[i] = -Q[i-1]*a[i]/b[i] + c[i]/b[i];
	}
	M[1] = -e[0]/d[0];
	L[1] = f[0]/d[0];
	for (int i = 1; i < N-1; i++) {
		M[i+1] = -M[i]*e[i]/d[i];
		L[i+1] = M[i]*f[i]/d[i] + L[i];
	}
	
	mat S = zeros<mat>(4,4);
	vec R, X;
	S << P[N-1] << 0 << -1 << 0 << endr <<
	     0	<<	1	<<	0 << -M[N-1] << endr <<
		ksi << 1	<< 0 << 0	<< endr <<
		0 <<   0	<< kapa << 1;
	R << -Q[N-1] << L[N-1] << eta << dzeta;
	X = solve(S,R);
	//cout << X;
	r1[0] = X(0);
	r2[0] = X(1);
	r1[N-1] = X(2);
	r2[N-1] = X(3);
	
	for(int i = 1; i < N; i++) {
		r1[i] = P[i] * r1[0] + Q[i];
	}
	for (int i = N-2; i >=0; i--) {
		r2[i] = (-r2[i+1]*e[i] + f[i])/d[i];
	}
	
	for (int k = 0; k < N; k++) {
		mesh->Values[k].v = (r1[k] + r2[k])/2/mesh->Values[k].getA(); //fix getA!
		mesh->Values[k].eps = (r2[k] - r1[k])/2;
		// fix x[k]!
	}
	return 0;
}
	
Node NumMethod::FirstOrder_First(Mesh *mesh) {
	Node tmp = mesh->Values[0];
	float w1, w2;
	float a_next = (mesh->Values[0].getA() + mesh->Values[1].getA())/2;;
	
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	w2 = mesh->Values[0].getRiman(2) + (mesh->Values[1].getRiman(2) - mesh->Values[0].getRiman(2))*(tau*a_next)/(mesh->Values[1].x - mesh->Values[0].x);
	w1 = -w2;
	tmp.eps = 2*w2;	
	return tmp;
}

Node NumMethod::FirstOrder_Last(Mesh *mesh) {
	Node tmp = mesh->Values[mesh->NumX - 1];
	float w1, w2;
	float a_prev = (mesh->Values[mesh->NumX - 1].getA() + mesh->Values[mesh->NumX - 2].getA())/2;
	
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	w1 = mesh->Values[mesh->NumX - 1].getRiman(1) + (mesh->Values[mesh->NumX - 1].getRiman(1) - mesh->Values[mesh->NumX - 2].getRiman(1))*(-tau*a_prev)/(mesh->Values[mesh->NumX - 1].x - mesh->Values[mesh->NumX - 2].x);
	w2 = -w1;
	tmp.eps = 2*w2;
	return tmp;
}

int NumMethod::FirstOrder(Mesh *mesh) {
	Node last = FirstOrder_First(mesh);
	Node tmp;
	float w1, w2;
	float a_next, a_prev;
	int i;
	
		for(i = 1; i < mesh->NumX - 1; i++) {
			a_next = (mesh->Values[i+1].getA() + mesh->Values[i].getA())/2;
			a_prev = (mesh->Values[i-1].getA() + mesh->Values[i].getA())/2;
			tmp = mesh->Values[i];
			tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;
			w1 = mesh->Values[i].getRiman(1) + (mesh->Values[i].getRiman(1) - mesh->Values[i-1].getRiman(1))*(-tau*a_prev)/(mesh->Values[i].x - mesh->Values[i-1].x);
			w2 = mesh->Values[i].getRiman(2) + (mesh->Values[i+1].getRiman(2) - mesh->Values[i].getRiman(2))*(tau*a_next)/(mesh->Values[i+1].x - mesh->Values[i].x);
			tmp.v = (w1 + w2)*(a_next + a_prev)/2;
			if(fabs(tmp.x - mesh->Values[i].x) > fmax(fabs(mesh->Values[i].x - mesh->Values[i-1].x), fabs(mesh->Values[i].x - mesh->Values[i+1].x))/3) { cout << "PIZDEC' X i="<< i << endl; return -1; } 
			tmp.eps = w2 - w1;
			if(fabs(tmp.eps) >= 0.000125) cout << "Plastic area" << endl;
			mesh->Values[i-1] = last;
			last = tmp;
		}
	mesh->Values[mesh->NumX - 1] = FirstOrder_Last(mesh);
	mesh->Values[mesh->NumX - 2] = last;
}

