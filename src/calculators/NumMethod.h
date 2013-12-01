#include "../Mesh.h"
#include "Approximator.h"
#include <gsl/gsl_poly.h>

class NumMethod {
	double tau;
	Approximator* proxima;
	Mesh* mesh;
	Mesh* tmpMesh;
	bool dx;
	
	public:
		NumMethod();
		NumMethod(Mesh* _mesh, Mesh* _tmpMesh, double _tau);
		~NumMethod();
		int FirstOrder(Node first, Node last);
		int SecondOrder(Node first, Node last);
		int TimeSecondOrder(Node first, Node last);
		Node FirstOrder_First(bool epsIsFix, double fixVal);
		Node SecondOrder_First(bool epsIsFix, double fixVal);
		Node FirstOrder_Last(bool epsIsFix, double fixVal);
		Node SecondOrder_Last(bool epsIsFix, double fixVal);
		int ImplicitSecondOrder(bool leftEpsIsFix, double leftFixVal, \
		bool rightEpsIsFix, double rightFixVal);
		double lAppA(int num, int order);
		double rAppA(int num, int order);
};

