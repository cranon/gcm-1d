#include "../Mesh.h"

using namespace std;

class Monitor {
	public:
		double tau;
		string rheology;
		double MaxV;
		Monitor();
		Monitor(int _Num, double _tau);
		Monitor(int _Num, double _tau, string _rheology);
		struct MonStruct monStruct;
		void getH(Mesh *mesh);
		void getCourant(Mesh *mesh);
		~Monitor();
};
