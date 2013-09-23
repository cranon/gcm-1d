#include "Node.h"
#include "Mesh.h"
#include <cmath>

class NumMethod {
	float tau;
	public:
		NumMethod();
		NumMethod(float _tau);
		~NumMethod();
		void FirstOrder(Mesh *mesh);
		Node FirstOrder_First(Mesh *mesh);
		Node FirstOrder_Last(Mesh *mesh);
};

