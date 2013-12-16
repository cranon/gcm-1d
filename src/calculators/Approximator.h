#include "../Mesh.h"
#include <armadillo>

using namespace arma;

class Approximator {
	Mesh* mesh;
public:
	Approximator(Mesh* _mesh);
	~Approximator();
	double LinearAppr(int i, vec* b, double x0);
	double LinearAppr(double x1, double y1, double x2, double y2, double x0);
	vec LinearAppr(int i, vec* b);
	double QuadraticAppr(int i, vec* b, double x0, bool limitor);
	vec QuadraticAppr(int i, vec* b);
};


