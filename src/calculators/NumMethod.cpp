#include "NumMethod.h"

using namespace std;

NumMethod::NumMethod() {
	
}

NumMethod::NumMethod(Mesh* _mesh, float _tau) {
	mesh = _mesh;
	tau = _tau;
	proxima = new Approximator(_mesh);
}

NumMethod::~NumMethod() {

}

float NumMethod::lAppA(int num, int order) {
	vec b;
	vec X;
	switch(order) {
		case 0:
			return mesh->Values[num].getA();
		case 1:
			b << mesh->Values[num-1].getA() << mesh->Values[num].getA();
			X = proxima->LinearAppr(num-1, &b);
			return (X(0)*mesh->Values[num].x + X(1))/(1 + X(0)*tau);
		case 2:
			double x1 = 0.0;
			double x2 = 0.0;
			double x0 = lAppA(num, 1);
			double h = 0.0;
			if(num == 1) {
				b << mesh->Values[0].getA() << mesh->Values[1].getA() << mesh->Values[2].getA();
				X = proxima->QuadraticAppr(0, &b);
				gsl_poly_solve_quadratic(X(0)*tau*tau, -(2*tau*X(0)*mesh->Values[1].x + tau*X(1) + 1), X(0)*mesh->Values[1].x*mesh->Values[1].x + X(1)*mesh->Values[1].x + X(2), &x1, &x2);
				if(x1*x2 == 0.0) return x0;
				if(fabs(x0 - x1) > fabs(x0 - x2)) x1 = x2;
					if(x1 >= fmin(mesh->Values[1].getA(), mesh->Values[0].getA()) && x1 <= fmax(mesh->Values[1].getA(), mesh->Values[0].getA())) return x1;
					else return x0;
			} else {
				b << mesh->Values[num-2].getA() << mesh->Values[num-1].getA() << mesh->Values[num].getA();
				X = proxima->QuadraticAppr(num-2, &b);
				gsl_poly_solve_quadratic(X(0)*tau*tau, -(2*tau*X(0)*mesh->Values[num].x + tau*X(1) + 1), X(0)*mesh->Values[num].x*mesh->Values[num].x + X(1)*mesh->Values[num].x + X(2), &x1, &x2);
				if(x1*x2 == 0) return x0;
				if(fabs(x0 - x1) > fabs(x0 - x2)) x1 = x2;
					h = mesh->Values[num].x - mesh->Values[num-1].x - x1*tau;
					if(h > 0) {
						if(x1 >= fmin(mesh->Values[num].getA(), mesh->Values[num-1].getA()) && x1 <= fmax(mesh->Values[num].getA(), mesh->Values[num-1].getA())) return x1;
						else return x0;
					} else if(h < 0) {
						cout << "Courant > 1 lAppA " << num << " tau = " << tau << endl;
						if(x1 >= fmin(mesh->Values[num-1].getA(), mesh->Values[num-2].getA()) && x1 <= fmax(mesh->Values[num-1].getA(), mesh->Values[num-2].getA())) return x1;
						else return x0;
					} else return mesh->Values[num-1].getA();
			}
	}
}

float NumMethod::rAppA(int num, int order) {
	vec b;
	vec X;
	switch(order) {
		case 0:
			return mesh->Values[num].getA();
		case 1:
			b << mesh->Values[num].getA() << mesh->Values[num+1].getA();
			X = proxima->LinearAppr(num, &b);
			return (X(0)*mesh->Values[num].x + X(1))/(1 - X(0)*tau);
		case 2:
			double x1 = 0.0;
			double x2 = 0.0;
			double x0 = rAppA(num, 1);
			double h = 0.0;
			if(num == mesh->NumX - 2) {
				b << mesh->Values[num-1].getA() << mesh->Values[num].getA() << mesh->Values[num+1].getA();
				X = proxima->QuadraticAppr(num-1, &b);
				gsl_poly_solve_quadratic(X(0)*tau*tau, 2*X(0)*tau*mesh->Values[num].x + X(1)*tau - 1, X(0)*mesh->Values[num].x*mesh->Values[num].x + X(1)*mesh->Values[num].x + X(2), &x1, &x2);
				if(x1*x2 == 0.0) return x0;
				if(fabs(x0 - x1) > fabs(x0 - x2)) x1 = x2;
					if(x1 >= fmin(mesh->Values[num].getA(), mesh->Values[num+1].getA()) && x1 <= fmax(mesh->Values[num].getA(), mesh->Values[num+1].getA())) return x1;
					else return x0;
			} else {
				b << mesh->Values[num].getA() << mesh->Values[num+1].getA() << mesh->Values[num+2].getA();
				X = proxima->QuadraticAppr(num, &b);
				gsl_poly_solve_quadratic(X(0)*tau*tau, 2*X(0)*tau*mesh->Values[num].x + X(1)*tau - 1, X(0)*mesh->Values[num].x*mesh->Values[num].x + X(1)*mesh->Values[num].x + X(2), &x1, &x2);
				if(x1*x2 == 0) return x0;
				if(fabs(x0 - x1) > fabs(x0 - x2)) x1 = x2; 
					h = mesh->Values[num+1].x - mesh->Values[num].x - x1*tau;
					if(h > 0) {
						if(x1 >= fmin(mesh->Values[num+1].getA(), mesh->Values[num].getA()) && x1 <= fmax(mesh->Values[num+1].getA(), mesh->Values[num].getA())) return x1;
						else return x0;
					} else if(h < 0) {
						cout << "Courant > 1 rAppA " << num << " tau = " << tau << endl;
						if(x1 >= fmin(mesh->Values[num+2].getA(), mesh->Values[num+1].getA()) && x1 <= fmax(mesh->Values[num+2].getA(), mesh->Values[num+1].getA())) return x1;
						else return x0;
					} else return mesh->Values[num+1].getA();

			}
	}
}

// First order for a first node
Node NumMethod::FirstOrder_First() { 
	Node tmp = mesh->Values[0];
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	vec b;
	float aR = rAppA(0, 1);
	b << mesh->Values[0].getRiman(2,aR) << mesh->Values[1].getRiman(2,aR);
	tmp.eps = proxima->LinearAppr(0, &b, mesh->Values[0].x + tau*aR)/aR;
	return tmp;
}

// Second order for a first node (zeroV)
Node NumMethod::SecondOrder_First() { 
	Node tmp = mesh->Values[0];
	tmp.x = mesh->Values[0].x + tau * mesh->Values[0].v;
	tmp.v = 0;
	vec b;
	float aR = rAppA(0, 2);
	b << mesh->Values[0].getRiman(2,aR) << mesh->Values[1].getRiman(2,aR) << mesh->Values[2].getRiman(2,aR);
	tmp.eps = proxima->QuadraticAppr(0, &b, mesh->Values[0].x + tau*aR, true)/aR;	
	return tmp;
}
// Second order for a first node (any corner condition)
Node NumMethod::SecondOrder_First(bool epsIsFix, float fixVal) { 
	Node tmp = mesh->Values[0];
	tmp.x = tmp.x + tau * tmp.v;
	vec b;
	float aR = rAppA(0, 2);
	b << mesh->Values[0].getRiman(2,aR) << mesh->Values[1].getRiman(2,aR) << mesh->Values[2].getRiman(2,aR);
	if (epsIsFix) {
		tmp.eps = fixVal;
		tmp.v = proxima->QuadraticAppr(0, &b, mesh->Values[0].x + tau*aR, true) - aR*fixVal;
	} else {
		tmp.v = fixVal;
		tmp.eps = (proxima->QuadraticAppr(0, &b, mesh->Values[0].x + tau*aR, true) - fixVal)/aR;	
	}
	return tmp;
}

// First order for a last node
Node NumMethod::FirstOrder_Last() {
	Node tmp = mesh->Values[mesh->NumX - 1];
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	vec b;
	float aL = lAppA(mesh->NumX-1,1);
	b << mesh->Values[mesh->NumX-2].getRiman(1,aL) << mesh->Values[mesh->NumX-1].getRiman(1,aL);
	tmp.eps = proxima->LinearAppr(mesh->NumX-2, &b, mesh->Values[mesh->NumX-1].x - tau*aL)/aL;
	return tmp;
}

// Second order for a last node (zeroV)
Node NumMethod::SecondOrder_Last() {
	Node tmp = mesh->Values[mesh->NumX - 1];
	tmp.x = mesh->Values[mesh->NumX - 1].x + tau * mesh->Values[mesh->NumX - 1].v;
	tmp.v = 0;
	vec b;
	float aL = lAppA(mesh->NumX-1, 2);
	b << mesh->Values[mesh->NumX-3].getRiman(1,aL) << mesh->Values[mesh->NumX-2].getRiman(1,aL) << mesh->Values[mesh->NumX-1].getRiman(1,aL);
	tmp.eps = proxima->QuadraticAppr(mesh->NumX-3, &b, mesh->Values[mesh->NumX-1].x - tau*aL, true)/aL;
	return tmp;
}

// Second order for a last node (any corner condition)
Node NumMethod::SecondOrder_Last(bool epsIsFix, float fixVal) {
	Node tmp = mesh->Values[mesh->NumX - 1];
	tmp.x = tmp.x + tau * tmp.v;
	vec b;
	float aL = lAppA(mesh->NumX-1, 2);
	b << mesh->Values[mesh->NumX-3].getRiman(1,aL) << mesh->Values[mesh->NumX-2].getRiman(1,aL) << mesh->Values[mesh->NumX-1].getRiman(1,aL);
	if (epsIsFix) {
		tmp.eps = fixVal;
		tmp.v = aL* fixVal - proxima->QuadraticAppr(mesh->NumX-3, &b, mesh->Values[mesh->NumX-1].x - tau*aL, true);
	} else {
		tmp.v = fixVal;
		tmp.eps = (proxima->QuadraticAppr(mesh->NumX-3, &b, mesh->Values[mesh->NumX-1].x - tau*aL, true) + fixVal)/aL;
	}
	return tmp;
}

// First order
int NumMethod::FirstOrder() {
	Node last = FirstOrder_First();
	Node tmp;
	float w1, w2;
	vec b;
	int i;
		for(i = 1; i < mesh->NumX - 1; i++) {
			tmp = mesh->Values[i];
			tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;
			
			float aL = lAppA(i, 1);
			float aR = rAppA(i, 1);
			b << mesh->Values[i-1].getRiman(1,aL) << mesh->Values[i].getRiman(1,aL);
			w1 = proxima->LinearAppr(i-1, &b, mesh->Values[i].x - tau*aL);
			
			b << mesh->Values[i].getRiman(2,aR) << mesh->Values[i+1].getRiman(2,aR);
			w2 = proxima->LinearAppr(i, &b, mesh->Values[i].x + tau*aR);
			
			tmp.v = (-w1*aR + w2*aL)/(aL + aR);
			tmp.eps = (w2 + w1)/(aL + aR);			
			// To indicate large displacement
			if(fabs(tmp.x - mesh->Values[i].x) > \
					fmax(fabs(mesh->Values[i].x - mesh->Values[i-1].x), \
					fabs(mesh->Values[i].x - mesh->Values[i+1].x))/3) { 
				cout << "Large displacement i = "<< i << endl;
				return -1; 
			} 
			
			mesh->Values[i-1] = last;
			last = tmp;
		}
	mesh->Values[mesh->NumX - 1] = FirstOrder_Last();
	mesh->Values[mesh->NumX - 2] = last;
}

int NumMethod::SecondOrder(Node first_node, Node last_node) {
	Node tmp;
	
	// 'temp1' is the first node
	Node temp1 = first_node; 	
	
	// 'temp2' is the second node with distorted 'w1' pattern
	Node temp2 = mesh->Values[1];
	temp2.x = mesh->Values[1].x + tau*mesh->Values[1].v;
	
	vec b;
	float aL = lAppA(1, 2);
	float aR = rAppA(1, 2);
	b << mesh->Values[0].getRiman(1,aL) << mesh->Values[1].getRiman(1,aL) << mesh->Values[2].getRiman(1,aL);
	float w1 = proxima->QuadraticAppr(0, &b, mesh->Values[1].x - tau*aL, true);

	
	b << mesh->Values[1].getRiman(2,aR) << mesh->Values[2].getRiman(2,aR) << mesh->Values[3].getRiman(2,aR);
	float w2 = proxima->QuadraticAppr(1, &b, mesh->Values[1].x + tau*aR, true);
	
	temp2.v = (-w1*aR + w2*aL)/(aL + aR);
	temp2.eps = (w2 + w1)/(aL + aR);
	
	// General calculation with standart pattern
	int i;
	for(i = 2; i < mesh->NumX - 2 ; i++) {
		tmp = mesh->Values[i];
		tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;
		
		aL = lAppA(i, 2);
		aR = rAppA(i, 2);
		b << mesh->Values[i-2].getRiman(1,aL) << mesh->Values[i-1].getRiman(1,aL) << mesh->Values[i].getRiman(1,aL);
		w1 = proxima->QuadraticAppr(i-2, &b, mesh->Values[i].x - tau*aL, false);
		
		
		b << mesh->Values[i].getRiman(2,aR) << mesh->Values[i+1].getRiman(2,aR) << mesh->Values[i+2].getRiman(2,aR);
		w2 = proxima->QuadraticAppr(i, &b, mesh->Values[i].x + tau*aR, false);
		
		//if(max(rAppA(i, 2), lAppA(i, 2)) > 2582.0) cout << "i = " << i << " " << max(rAppA(i, 2), lAppA(i, 2)) << endl;;
		
		tmp.v = (-w1*aR + w2*aL)/(aL + aR);
		tmp.eps = (w2 + w1)/(aL + aR);
		
		mesh->Values[i-2] = temp1;
		temp1 = temp2;
		temp2 = tmp;
	}
	
	// 'tmp' is the last but one node with distorted 'w2' pattern
	tmp = mesh->Values[i];
	tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v;

	aL = lAppA(i, 2);
	aR = rAppA(i, 2);
	
	b << mesh->Values[i-2].getRiman(1,aL) << mesh->Values[i-1].getRiman(1,aL) << mesh->Values[i].getRiman(1,aL);
	w1 = proxima->QuadraticAppr(i-2, &b, mesh->Values[i].x - tau*aL, true);
	
	
	b << mesh->Values[i-1].getRiman(2,aR) << mesh->Values[i].getRiman(2,aR) << mesh->Values[i+1].getRiman(2,aR);
	w2 = proxima->QuadraticAppr(i-1, &b, mesh->Values[i].x + tau*aR, true);
	
	tmp.v = (-w1*aR + w2*aL)/(aL + aR);
	tmp.eps = (w2 + w1)/(aL + aR);
		
	mesh->Values[i-2] = temp1;
	
	// The last node
	mesh->Values[i+1] = last_node;
	mesh->Values[i-1] = temp2;
	mesh->Values[i] = tmp;
}

int NumMethod::ImplicitSecondOrder() {
	int N = mesh->NumX;
	float a[N], b[N], c[N], d[N], e[N], f[N], P[N], Q[N], M[N], L[N], r1[N], r2[N];
	for (int k = 1; k < N; k++) {
		float A = 0.5*(mesh->Values[k].getA() + mesh->Values[k-1].getA());
		float g1 = 0; // fix normal calculating g!
		float g2 = 0; // fix normal calculating g!
		float h = mesh->Values[k].x - mesh->Values[k-1].x;
		a[k] = 1 - A*tau/h;
		b[k] = 1 + A*tau/h;
		c[k] = 2*A*tau*g1 - 2*tau*g2 + a[k]*mesh->Values[k].getRiman(1,A) + \
				b[k]*mesh->Values[k-1].getRiman(1,A);
	}
	for (int k = 0; k < N-1; k++) {
		float A = 0.5*(mesh->Values[k].getA() + mesh->Values[k+1].getA());
		vec b;
		float g1 = 0; // fix normal calculating g!
		float g2 = 0; // fix normal calculating g!
		float h = mesh->Values[k+1].x - mesh->Values[k].x;
		e[k] = 1 - A*tau/h;
		d[k] = 1 + A*tau/h;
		f[k] = 2*A*tau*g1 + 2*tau*g2 + e[k]*mesh->Values[k].getRiman(2,A) + \
				d[k]*mesh->Values[k+1].getRiman(2,A);
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
	
	float a_l = mesh->Values[0].getA();
	float a_r = (mesh->Values[0].getA() + mesh->Values[1].getA())/2;
	mesh->Values[0].v = (a_l*r2[0] - a_r*r1[0])/(a_l + a_r);
	mesh->Values[0].eps = (r1[0] + r2[0])/(a_l + a_r);
	// fix x[0]!
	for (int k = 1; k < N-1; k++) {
		a_l = (mesh->Values[k].getA() + mesh->Values[k-1].getA())/2;
		a_r = (mesh->Values[k].getA() + mesh->Values[k+1].getA())/2;
		mesh->Values[k].v = (a_l * r2[k] - a_r * r1[k])/(a_l + a_r);
		mesh->Values[k].eps = (r1[k] + r2[k]) / (a_l + a_r);
		// fix x[k]!
	}
	a_r = mesh->Values[N-1].getA();
	a_l = (mesh->Values[N-1].getA() + mesh->Values[N-2].getA())/2;
	mesh->Values[N-1].v = (a_l * r2[N-1] - a_r * r1[N-1])/(a_l + a_r);
	mesh->Values[N-1].eps = (r1[N-1] + r2[N-1]) / (a_l + a_r);
	//fix x[N-1]!
	return 0;
}
