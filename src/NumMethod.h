#include "Mesh.h"

class NumMethod {
	float tau;
	public:
		NumMethod();
		NumMethod(float _tau);
		~NumMethod();
		int FirstOrder(Mesh *mesh);
		Node FirstOrder_First(Mesh *mesh);
		Node FirstOrder_Last(Mesh *mesh);
};

