#include "../Mesh.h"
#include <armadillo>

using namespace arma;

class Approximator {
	Mesh* mesh;
public:
	Approximator(Mesh* _mesh);
	~Approximator();
	float LinearAppr(int i, vec* b, float x0);
	vec LinearAppr(int i, vec* b);
	float QuadraticAppr(int i, vec* b, float x0, bool limitor);
	vec QuadraticAppr(int i, vec* b);
};


