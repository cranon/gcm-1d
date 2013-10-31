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
		int FirstOrder();
		Node FirstOrder_First();
		Node SecondOrder_First();
		Node SecondOrder_First(bool epsIsFix, float fixVal);
		Node FirstOrder_Last();
		Node SecondOrder_Last();
		Node SecondOrder_Last(bool epsIsFix, float fixVal);
		int ImplicitSecondOrder(); 
		int SecondOrder(Node first, Node last);
		float lAppA(int num, int order);
		float rAppA(int num, int order);
};

