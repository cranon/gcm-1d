#include "NumMethod.h"

using namespace std;

NumMethod::NumMethod() {
	
}

NumMethod::NumMethod(Mesh* _mesh, Mesh* _tmpMesh, double _tau) {
	dx = true;
	mesh = _mesh;
	tmpMesh = _tmpMesh;
	tau = _tau;
	proxima = new Approximator(_mesh);
}

NumMethod::~NumMethod() {

}

double NumMethod::lAppA(int num, int order) {
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

double NumMethod::rAppA(int num, int order) {
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

// First order for a first node (any corner condition)
Node NumMethod::FirstOrder_First(bool epsIsFix, double fixVal) { 
	Node tmp = mesh->Values[0];
	tmp.x = tmp.x + tau * tmp.v * dx;
	vec b;
	double aR = rAppA(0, 1);
	b << mesh->Values[0].getRiman(2,aR) << mesh->Values[1].getRiman(2,aR);
	if (epsIsFix) {
		tmp.eps = fixVal;
		tmp.v = proxima->LinearAppr(0, &b, mesh->Values[0].x + tau*aR) - aR*fixVal;
	} else {
		tmp.v = fixVal;
		tmp.eps = (proxima->LinearAppr(0, &b, mesh->Values[0].x + tau*aR) - fixVal)/aR;	
	}
	return tmp;
}

// Second order for a first node (any corner condition)
Node NumMethod::SecondOrder_First(bool epsIsFix, double fixVal) { 
	Node tmp = mesh->Values[0];
	double prev_v = tmp.v;
	vec b;
	double aR = rAppA(0, 2);
	b << mesh->Values[0].getRiman(2,aR) << mesh->Values[1].getRiman(2,aR) << mesh->Values[2].getRiman(2,aR);
	if (epsIsFix) {
		tmp.eps = fixVal;
		tmp.v = proxima->QuadraticAppr(0, &b, mesh->Values[0].x + tau*aR, true) - aR*fixVal;
	} else {
		tmp.v = fixVal;
		tmp.eps = (proxima->QuadraticAppr(0, &b, mesh->Values[0].x + tau*aR, true) - fixVal)/aR;	
	}
	tmp.x = tmp.x + dx * tau * (tmp.v + prev_v) / 2;
	return tmp;
}

// First order for a last node (any corner condition)
Node NumMethod::FirstOrder_Last(bool epsIsFix, double fixVal) {
	Node tmp = mesh->Values[mesh->NumX - 1];
	tmp.x = tmp.x + tau * tmp.v * dx;
	vec b;
	double aL = lAppA(mesh->NumX-1,1);
	b << mesh->Values[mesh->NumX-2].getRiman(1,aL) << mesh->Values[mesh->NumX-1].getRiman(1,aL);
	if (epsIsFix) {
		tmp.eps = fixVal;
		tmp.v = aL* fixVal - proxima->LinearAppr(mesh->NumX-2, &b, mesh->Values[mesh->NumX-1].x - tau*aL);
	} else {
		tmp.v = fixVal;
		tmp.eps = (proxima->LinearAppr(mesh->NumX-2, &b, mesh->Values[mesh->NumX-1].x - tau*aL) + fixVal)/aL;
	}
	return tmp;
}

// Second order for a last node (any corner condition)
Node NumMethod::SecondOrder_Last(bool epsIsFix, double fixVal) {
	Node tmp = mesh->Values[mesh->NumX - 1];
	double prev_v = tmp.v;
	vec b;
	double aL = lAppA(mesh->NumX-1, 2);
	b << mesh->Values[mesh->NumX-3].getRiman(1,aL) << mesh->Values[mesh->NumX-2].getRiman(1,aL) << mesh->Values[mesh->NumX-1].getRiman(1,aL);
	if (epsIsFix) {
		tmp.eps = fixVal;
		tmp.v = aL* fixVal - proxima->QuadraticAppr(mesh->NumX-3, &b, mesh->Values[mesh->NumX-1].x - tau*aL, true);
	} else {
		tmp.v = fixVal;
		tmp.eps = (proxima->QuadraticAppr(mesh->NumX-3, &b, mesh->Values[mesh->NumX-1].x - tau*aL, true) + fixVal)/aL;
	}
	tmp.x = tmp.x + dx * tau * (tmp.v + prev_v) / 2;
	return tmp;
}

// First order
int NumMethod::FirstOrder(Node first_node, Node last_node) {
	Node last = first_node;
	Node tmp;
	double w1, w2;
	vec b;
	int i;
		for(i = 1; i < mesh->NumX - 1; i++) {
			tmp = mesh->Values[i];
			tmp.x = mesh->Values[i].x + tau*mesh->Values[i].v * dx;
			
			double aL = lAppA(i, 1);
			double aR = rAppA(i, 1);
			b << mesh->Values[i-1].getRiman(1,aL) << mesh->Values[i].getRiman(1,aL);
			w1 = proxima->LinearAppr(i-1, &b, mesh->Values[i].x - tau*aL);
			
			b << mesh->Values[i].getRiman(2,aR) << mesh->Values[i+1].getRiman(2,aR);
			w2 = proxima->LinearAppr(i, &b, mesh->Values[i].x + tau*aR);
			
			tmp.v = (-w1*aR + w2*aL)/(aL + aR);
			tmp.eps = (w2 + w1)/(aL + aR);
			
			mesh->Values[i-1] = last;
			last = tmp;
		}
	mesh->Values[mesh->NumX - 1] = last_node;
	mesh->Values[mesh->NumX - 2] = last;
}

int NumMethod::SecondOrder(Node first_node, Node last_node) {
	Node tmp;
	
	// 'temp1' is the first node
	Node temp1 = first_node; 	
	
	// 'temp2' is the second node with distorted 'w1' pattern
	Node temp2 = mesh->Values[1];
	double prev_v = temp2.v;
	
	vec b;
	double aL = lAppA(1, 2);
	double aR = rAppA(1, 2);
	b << mesh->Values[0].getRiman(1,aL) << mesh->Values[1].getRiman(1,aL) << mesh->Values[2].getRiman(1,aL);
	double w1 = proxima->QuadraticAppr(0, &b, mesh->Values[1].x - tau*aL, true);

	
	b << mesh->Values[1].getRiman(2,aR) << mesh->Values[2].getRiman(2,aR) << mesh->Values[3].getRiman(2,aR);
	double w2 = proxima->QuadraticAppr(1, &b, mesh->Values[1].x + tau*aR, true);
	
	temp2.v = (-w1*aR + w2*aL)/(aL + aR);
	temp2.eps = (w2 + w1)/(aL + aR);
	temp2.x = temp2.x + dx * tau * (temp2.v + prev_v) / 2;
	
	// General calculation with standart pattern
	int i;
	for(i = 2; i < mesh->NumX - 2 ; i++) {
		tmp = mesh->Values[i];
		prev_v = tmp.v;
		
		aL = lAppA(i, 2);
		aR = rAppA(i, 2);
		b << mesh->Values[i-2].getRiman(1,aL) << mesh->Values[i-1].getRiman(1,aL) << mesh->Values[i].getRiman(1,aL);
		w1 = proxima->QuadraticAppr(i-2, &b, mesh->Values[i].x - tau*aL, true);
		
		
		b << mesh->Values[i].getRiman(2,aR) << mesh->Values[i+1].getRiman(2,aR) << mesh->Values[i+2].getRiman(2,aR);
		w2 = proxima->QuadraticAppr(i, &b, mesh->Values[i].x + tau*aR, true);
		
		tmp.v = (-w1*aR + w2*aL)/(aL + aR);
		tmp.eps = (w2 + w1)/(aL + aR);
		tmp.x = tmp.x + dx * tau * (tmp.v + prev_v) / 2;
		
		mesh->Values[i-2] = temp1;
		temp1 = temp2;
		temp2 = tmp;
	}
	
	// 'tmp' is the last but one node with distorted 'w2' pattern
	tmp = mesh->Values[i];
	prev_v = tmp.v;
	
	aL = lAppA(i, 2);
	aR = rAppA(i, 2);
	
	b << mesh->Values[i-2].getRiman(1,aL) << mesh->Values[i-1].getRiman(1,aL) << mesh->Values[i].getRiman(1,aL);
	w1 = proxima->QuadraticAppr(i-2, &b, mesh->Values[i].x - tau*aL, true);
	
	
	b << mesh->Values[i-1].getRiman(2,aR) << mesh->Values[i].getRiman(2,aR) << mesh->Values[i+1].getRiman(2,aR);
	w2 = proxima->QuadraticAppr(i-1, &b, mesh->Values[i].x + tau*aR, true);
	
	tmp.v = (-w1*aR + w2*aL)/(aL + aR);
	tmp.eps = (w2 + w1)/(aL + aR);
	tmp.x = tmp.x + dx * tau * (tmp.v + prev_v) / 2;

	mesh->Values[i-2] = temp1;
	
	// The last node
	mesh->Values[i+1] = last_node;
	mesh->Values[i-1] = temp2;
	mesh->Values[i] = tmp;
}

int NumMethod::TimeSecondOrder(Node first_node, Node last_node) {
	Mesh* tmp = mesh;
	mesh = tmpMesh;
	tmpMesh = tmp;
	tau = tau/2;
	SecondOrder(first_node, last_node);
	tmp = mesh;
	mesh = tmpMesh;
	tmpMesh = tmp;
	for(int i = 0; i < mesh->NumX; i++) {
		mesh->Values[i].x = tmpMesh->Values[i].x;
	}
	
	dx = false;
	tau = 2*tau;
	SecondOrder(first_node, last_node);
	
	tau = tau/2;
	for(int i = 0; i < mesh->NumX; i++) {
		mesh->Values[i].x = mesh->Values[i].x + (mesh->Values[i].v + \
				tmpMesh->Values[i].v) * tau / 2;
	}
	
	tau = 2*tau;
	dx = true;
	return 0;
}


int NumMethod::ImplicitSecondOrder(bool leftEpsIsFix, double leftFixVal, \
		bool rightEpsIsFix, double rightFixVal) {
	int N = mesh->NumX;
	double a[N], b[N], c[N], d[N], e[N], f[N], P[N], Q[N], M[N], L[N], r1[N], r2[N];
	double ksi, eta, dzeta, kapa;
	for (int k = 1; k < N; k++) {
		double A = 0.5*(mesh->Values[k].getA() + mesh->Values[k-1].getA());
		double g1 = 0; // fix normal calculating g!
		double g2 = 0; // fix normal calculating g!
		double h = mesh->Values[k].x - mesh->Values[k-1].x;
		a[k] = 1 - A*tau/h;
		b[k] = 1 + A*tau/h;
		c[k] = -2*tau*g1 + 2*A*tau*g2 + a[k]*mesh->Values[k].getRiman(1,A) + \
				b[k]*mesh->Values[k-1].getRiman(1,A);
	}
	for (int k = 0; k < N-1; k++) {
		double A = 0.5*(mesh->Values[k].getA() + mesh->Values[k+1].getA());
		vec b;
		double g1 = 0; // fix normal calculating g!
		double g2 = 0; // fix normal calculating g!
		double h = mesh->Values[k+1].x - mesh->Values[k].x;
		e[k] = 1 - A*tau/h;
		d[k] = 1 + A*tau/h;
		f[k] = 2*tau*g1 + 2*A*tau*g2 + e[k]*mesh->Values[k].getRiman(2,A) + \
				d[k]*mesh->Values[k+1].getRiman(2,A);
	}
	
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
	
	
	double a_l = mesh->Values[0].getA();
	double a_r = (mesh->Values[0].getA() + mesh->Values[1].getA())/2;
	if(leftEpsIsFix) {
		ksi = 1;
		eta = leftFixVal*(a_l+a_r);
	} else {
		ksi = -a_r/a_l;
		eta = leftFixVal*(a_l+a_r)/a_l;
	}
	a_r = mesh->Values[N-1].getA();
	a_l = (mesh->Values[N-1].getA() + mesh->Values[N-2].getA())/2;
	if(rightEpsIsFix) {
		kapa = 1;
		dzeta = rightFixVal*(a_l+a_r);
	} else {
		kapa = -a_r/a_l;
		dzeta = rightFixVal*(a_l+a_r)/a_l;
	}
	
	mat S = zeros<mat>(4,4);
	vec R, X;
	S << P[N-1] << 0 << -1 << 0 << endr <<
	     0	<<	1	<<	0 << -M[N-1] << endr <<
		ksi << 1	<< 0 << 0	<< endr <<
		0 <<   0	<< kapa << 1;
	R << -Q[N-1] << L[N-1] << eta << dzeta;
	X = solve(S,R);

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

	double gamma = 0;
	double tmp1, tmp2 = r1[0];
	for (int i = 1; i < N-1; i++) {
		double alpha = 0.1;
		if (fabs(tmp2 - 2*r1[i] + r1[i+1]) < gamma*fabs(r1[i] - tmp2)) alpha = 0.5;
		tmp1 = r1[i];
		r1[i] = r1[i]*(1 - 2*alpha) + alpha*tmp2 + alpha*r1[i+1];
		tmp2 = tmp1;
	}
	tmp2 = r2[0];
	for (int i = 1; i < N-1; i++) {
		double alpha = 0.1;
		if (fabs(tmp2 - 2*r2[i] + r2[i+1]) < gamma*fabs(r2[i+1] - r2[i])) alpha = 0.5;
		tmp1 = r2[i];
		r2[i] = r2[i]*(1 - 2*alpha) + alpha*tmp2 + alpha*r2[i+1];
		tmp2 = tmp1;
	}
	
	a_l = mesh->Values[0].getA();
	a_r = (mesh->Values[0].getA() + mesh->Values[1].getA())/2;
	mesh->Values[0].v = (a_l*r2[0] - a_r*r1[0])/(a_l + a_r);
	mesh->Values[0].eps = (r1[0] + r2[0])/(a_l + a_r);
	for (int k = 1; k < N-1; k++) {
		a_l = (mesh->Values[k].getA() + mesh->Values[k-1].getA())/2;
		a_r = (mesh->Values[k].getA() + mesh->Values[k+1].getA())/2;
		mesh->Values[k].v = (a_l * r2[k] - a_r * r1[k])/(a_l + a_r);
		mesh->Values[k].eps = (r1[k] + r2[k]) / (a_l + a_r);
	}
	a_r = mesh->Values[N-1].getA();
	a_l = (mesh->Values[N-1].getA() + mesh->Values[N-2].getA())/2;
	mesh->Values[N-1].v = (a_l * r2[N-1] - a_r * r1[N-1])/(a_l + a_r);
	mesh->Values[N-1].eps = (r1[N-1] + r2[N-1]) / (a_l + a_r);
	return 0;
}
