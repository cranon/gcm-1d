#include "Approximator.h"

Approximator::Approximator(Mesh* _mesh) {
	mesh = _mesh;
}

Approximator::~Approximator() {
	
}

double Approximator::LinearAppr(int i, vec* b, double x0) {
	if(x0 > mesh->Values[i+1].x || x0 < mesh->Values[i].x) {
		cerr << "LinearAppr: wrong 'x0' in " << i << " node" << endl;
		exit(-1); // STOP
	}
	mat A;
	A << mesh->Values[i].x << 1 << endr
		<< mesh->Values[i+1].x << 1 << endr;
	vec X = solve(A, *b);
	return X(0)*x0 + X(1);
	
}

double Approximator::LinearAppr(double x1, double y1, double x2, double y2, double x0) {
	return (y1*(x2 - x0) + y2*(x0 - x1))/(x2 - x1);
}

vec Approximator::LinearAppr(int i, vec* b) {
	mat A;
	A << mesh->Values[i].x << 1 << endr
		<< mesh->Values[i+1].x << 1 << endr;
	return solve(A, *b);
}

double Approximator::QuadraticAppr(int i, vec* b, double x0, bool limitor) {
	if(x0 > mesh->Values[i+2].x || x0 < mesh->Values[i].x) {
		cerr << "QuadraticAppr: wrong 'x0' in " << i << " node" << endl;
		exit(-1); // STOP
	}
	
	double x1 = -1;
	double x2 = 0;
	double x3 = (mesh->Values[i+2].x - mesh->Values[i+1].x) / (mesh->Values[i+1].x - mesh->Values[i].x);
	x0 = (x0 - mesh->Values[i+1].x) / (mesh->Values[i+1].x - mesh->Values[i].x);
	
	mat A;
	A << x1*x1 << x1 << 1.0 << endr
		<< x2*x2 << x2 << 1.0 << endr
		<< x3*x3 << x3 << 1.0 << endr;
		
	vec X = solve(A, *b);
	//if(abs(X(0)) < 0.001*abs(((*b)(2) - (*b)(0))/(x3 + 1))) return LinearAppr(x1, (*b)(0), x3, (*b)(2), x0);
	// Limitor
	if(!limitor) 
		return X(0)*x0*x0 + X(1)*x0 + X(2);
	else {
		double X0 = -X(1)/2/X(0);
		double res = X(0)*x0*x0 + X(1)*x0 + X(2);
		if(X0 >= x3 || X0 <= x1) return res;
		else if(x0 <= 0 && X0 < 0) {
			if(res < fmin((*b)(0), (*b)(1))) {
				//cout << X(0) << endl << ((*b)(2) - (*b)(0))/(x3 + 1)  << endl << endl;
				//cout << i << " step 1" << endl;
				return fmin((*b)(0), (*b)(1));
			} else if(res > fmax((*b)(0), (*b)(1))) {
				//cout << X(0) << endl << ((*b)(2) - (*b)(0))/(x3 + 1) << endl << endl;
				//cout << i << " step 2" << endl;
				return fmax((*b)(0), (*b)(1));
			}
			return res;
		} else if(x0 >= 0 && X0 > 0) {
			if(res < fmin((*b)(2), (*b)(1))) {
				//cout << i << " step 3" << endl;
				return fmin((*b)(2), (*b)(1));
			} else if(res > fmax((*b)(2), (*b)(1))) {
				//cout << i << " step 4" << endl;
				return fmax((*b)(2), (*b)(1));
			}
			return res;
		} else {
			return res;
		}
	}
		/*else {
		double X0 = -X(1)/2/X(0);
		if(X0 >= x3 || X0 <= x1) return X(0)*x0*x0 + X(1)*x0 + X(2);
		else if(x0 <= 0 && X0 < 0) {
			vec c = b->rows(0, 1);
			return this->LinearAppr(i, &c, \
			x0*(mesh->Values[i+1].x - mesh->Values[i].x) + mesh->Values[i+1].x);
		} else if(x0 >= 0 && X0 > 0) {
			vec c = b->rows(1, 2);
			return this->LinearAppr(i+1, &c, \
			x0*(mesh->Values[i+1].x - mesh->Values[i].x) + mesh->Values[i+1].x);
		} else {
			return X(0)*x0*x0 + X(1)*x0 + X(2);
		}
	}*/

}

vec Approximator::QuadraticAppr(int i, vec* b) {
	double x1 = -1;
	double x2 = 0;
	double x3 = (mesh->Values[i+2].x - mesh->Values[i+1].x) / (mesh->Values[i+1].x - mesh->Values[i].x);
	
	mat A;
	A << x1*x1 << x1 << 1.0 << endr
		<< x2*x2 << x2 << 1.0 << endr
		<< x3*x3 << x3 << 1.0 << endr;
	vec X = solve(A, *b);
	vec Y;
	Y << X(0)/(mesh->Values[i+1].x - mesh->Values[i].x)/(mesh->Values[i+1].x - mesh->Values[i].x)
		<< (X(1)*(mesh->Values[i+1].x - mesh->Values[i].x) - 2*X(0)*mesh->Values[i+1].x)/ \
			(mesh->Values[i+1].x - mesh->Values[i].x)/(mesh->Values[i+1].x - mesh->Values[i].x)
		<< X(2) + X(0)*mesh->Values[i+1].x*mesh->Values[i+1].x/(mesh->Values[i+1].x - mesh->Values[i].x)/(mesh->Values[i+1].x - mesh->Values[i].x)- \
			X(1)*mesh->Values[i+1].x/(mesh->Values[i+1].x - mesh->Values[i].x);
	return Y;
}
