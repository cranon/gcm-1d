#include "../Mesh.h"
#include <armadillo>

using namespace arma;

class Approximator {
	Mesh* mesh;
public:
	Approximator(Mesh* _mesh);
	~Approximator();
	float LinearAppr(int i, float f1, float f2, float x0);
	float QuadraticAppr(int i, vec* b, float x0, bool limitor);
};


