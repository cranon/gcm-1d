#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "Scene.h"

using namespace std;

int main(int argc, char** argv) {
	int opt = 0;
	float MaxTau = 0;
	int NumT = 0;
	int methodType = 1;
	while ((opt = getopt(argc, argv, "n:t:m:")) != -1) {
		switch (opt) {
			case 'm' : methodType = atoi(optarg); break;
			case 'n' : NumT = atoi(optarg); break;
			case 't' : MaxTau = atof(optarg); break;
			case '?' : exit(-1);
		}
	}
	Scene scene;
	scene.Init(1,"",false);
	for (int i = 0; i < NumT; i++) {
		if(scene.doNextStep(MaxTau, methodType) == -1) { 
			cerr << "main: error on " << i << " time step" << endl; 
			return -1; 
		}
	}
	return 0;
}

