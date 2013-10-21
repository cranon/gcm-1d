#include "Parser.h"
#include "calculators/NumMethod.h"
#include "calculators/Monitor.h"
#ifndef BODY_H
#define	BODY_H

using namespace std;

class Body {
	Mesh mesh;
	Mesh _mesh;
	string rheology;
	char fileName[100];
public:
	Body();
	int doNextStep(float tau, int methodNumber, \
					const char * _LeftCnrCond, const char * RightCnrCond);
	void setParameters(const char *fileName);
	void printData(int fileNumber);
	void printData(int fileNumber, struct MonStruct *monStruct);
	~Body();
	friend class Scene;

};

#endif	/* BODY_H */

