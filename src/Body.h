#include "Parser.h"
#include "NumMethod.h"
#include "Monitor.h"
#ifndef BODY_H
#define	BODY_H

using namespace std;

class Body {
	Mesh mesh;
	string rheology;
	string LeftCnrCond, RightCnrCond;
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

