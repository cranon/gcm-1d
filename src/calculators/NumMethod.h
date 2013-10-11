#include "../Mesh.h"
#include "Approximator.h"

class NumMethod {
	float tau;
	
	// Every NumMethod has his own approximator
	Approximator* proxima;
	
	Mesh* mesh;
	public:
		NumMethod();
		NumMethod(Mesh* _mesh, float _tau);
		~NumMethod();
		int FirstOrder();
		Node FirstOrder_First();
		Node SecondOrder_First();
		Node FirstOrder_Last();
		Node SecondOrder_Last();
		int ImplicitSecondOrder(Mesh *mesh); 
		int SecondOrder();
		float lAppA(int num, int order);
		float rAppA(int num, int order);
};

