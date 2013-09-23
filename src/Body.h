#include "Mesh.h"
#include "Parser.h"
#include "NumMethod.h"
#ifndef BODY_H
#define	BODY_H

class Body {
	Mesh mesh;
	const char *rheology;
	const char *LeftCnrCond, *RightCnrCond;
	char fileName[100];
public:
	Body();
	void doNextStep(float tau, int methodNumber, \
					const char * _LeftCnrCond, const char * RightCnrCond);
	void setParameters(const char *fileName);
	void printData(int fileNumber);
	~Body();

};

#endif	/* BODY_H */

