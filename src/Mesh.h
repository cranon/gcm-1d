#include "Node.h"
#include <fstream>
#ifndef MESH_H
#define	MESH_H

using namespace std;

struct MonStruct {
	int Num;
	float *monArr;
	float mean, min, max;	
};

class Mesh {
    Node *Values;
    string rheology;
public:
    int NumX;
    Mesh();
    void setParameters(Node *InitValues, int _NumX, string _rheology);
	void printData(const char *fileName);
	void printData(const char *fileName, struct MonStruct *monStruct);
	friend class NumMethod;
	friend class Approximator;
	friend class Monitor;
    ~Mesh();
};

#endif	/* MESH_H */

