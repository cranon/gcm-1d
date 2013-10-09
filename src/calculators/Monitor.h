#include "../Mesh.h"

using namespace std;

class Monitor {
	public:
		float tau;
		string rheology;
		float MaxV;
		Monitor();
		Monitor(int _Num, float _tau);
		Monitor(int _Num, float _tau, string _rheology);
		struct MonStruct monStruct;
		void getH(Mesh *mesh);
		void getCourant(Mesh *mesh);
		~Monitor();
};
