#include "Parser.h"
#include "Node.h"
#include "calculators/NumMethod.h"
#include "calculators/Monitor.h"
#ifndef BODY_H
#define	BODY_H

using namespace std;

class Body {
	double t;
	CnrCondition Left;
	CnrCondition Right;
	Mesh mesh;
	Mesh _mesh;
	Mesh tmpMesh;
	string rheology;
	char fileName[100];
public:
	Body();
	int doNextStep(double tau, int methodNumber, \
					const char * _LeftCnrCond, const char * RightCnrCond);
	void setParameters(const char *fileName);
	void printData(int fileNumber);
	void printData(int fileNumber, struct MonStruct *monStruct);
	void printRheology();
	~Body();
	friend class Scene;

};

#endif	/* BODY_H */

