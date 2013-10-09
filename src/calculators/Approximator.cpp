#include "Approximator.h"

Approximator::Approximator(Mesh* _mesh, bool _limitor) {
	mesh = _mesh;
	limitor = _limitor;
}

Approximator::~Approximator() {
	
}

float Approximator::LinearAppr(int i, float f1, float f2, float x0) {
	if(x0 > mesh->Values[i+1].x || x0 < mesh->Values[i].x) {
		cerr << "LinearAppr: wrong 'x0' in " << i << " node" << endl;
		return -1.0;
	}
	return (f1*(mesh->Values[i+1].x - x0) + f2*(x0 - mesh->Values[i].x))/(mesh->Values[i+1].x - mesh->Values[i].x);
	
}


float Approximator::QuadraticAppr(int i, vec* b, float x0) {
	if(x0 > mesh->Values[i+2].x || x0 < mesh->Values[i].x) {
		cerr << "QuadraticAppr: wrong 'x0' in " << i << " node" << endl;
		return -1.0;
	}
	mat A;
	A << (mesh->Values[i].x)*(mesh->Values[i].x) << mesh->Values[i].x << 1 << endr 
		<< (mesh->Values[i+1].x)*(mesh->Values[i+1].x) << mesh->Values[i+1].x << 1 << endr
		<< (mesh->Values[i+2].x)*(mesh->Values[i+2].x) << mesh->Values[i+2].x << 1 << endr;
	vec X = solve(A, *b);
	
	// Limitor
	if(!limitor) return X(0)*x0*x0 + X(1)*x0 + X(2);
	else {
		float X0 = -X(1)/2/X(0);
		if(X0 >= mesh->Values[i+2].x || X0 <= mesh->Values[i].x) {
/*			if(fabs(X0) >= 0.1) {						// Almost straight
				if(mesh->Values[i+1].x - x0 >= 0) {
					vec c = *b;
					return this->LinearAppr(i, c(0), c(1), x0);
				} else {
					vec c = *b;
					return this->LinearAppr(i+1, c(1), c(2), x0);
				}
			} else*/ return X(0)*x0*x0 + X(1)*x0 + X(2);
		} else if(mesh->Values[i+1].x - x0 >= 0 && mesh->Values[i+1].x - X0 > 0) {
			vec c = *b;
			return this->LinearAppr(i, c(0), c(1), x0);
		} else if(mesh->Values[i+1].x - x0 <= 0 && mesh->Values[i+1].x - X0 < 0) {
			vec c = *b;
			return this->LinearAppr(i+1, c(1), c(2), x0);
		} else return X(0)*x0*x0 + X(1)*x0 + X(2);
	}
}
