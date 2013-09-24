#include "Node.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#ifndef MESH_H
#define	MESH_H

using namespace std;

class Mesh {
    Node* Values;
    int NumX;
    string rheology;
public:
    Mesh();
    void setParameters(Node *InitValues, int _NumX, string _rheology);
	void printData(const char * fileName);
    ~Mesh();
	friend class NumMethod;
};

#endif	/* MESH_H */

