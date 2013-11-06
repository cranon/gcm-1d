#include "../Mesh.h"
#include "Approximator.h"
#include <gsl/gsl_poly.h>

class NumMethod {
	float tau;
	Approximator* proxima;
	Mesh* mesh;
	
	public:
		NumMethod();
		NumMethod(Mesh* _mesh, float _tau);
		~NumMethod();
		int FirstOrder(Node first, Node last);
		int SecondOrder(Node first, Node last);
		Node FirstOrder_First(bool epsIsFix, float fixVal);
		Node SecondOrder_First(bool epsIsFix, float fixVal);
		Node FirstOrder_Last(bool epsIsFix, float fixVal);
		Node SecondOrder_Last(bool epsIsFix, float fixVal);
		int ImplicitSecondOrder(bool leftEpsIsFix, float leftFixVal, \
		bool rightEpsIsFix, float rightFixVal);
		float lAppA(int num, int order);
		float rAppA(int num, int order);
};

