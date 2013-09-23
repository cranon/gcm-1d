#include "Node.h"
#ifndef MESH_H
#define	MESH_H

class Mesh {
    Node* Values;
    int NumX;
public:
    Mesh();
    void setParameters(Node *InitValues, int _NumX);
	void printData(const char * fileName);
    ~Mesh();
	friend class NumMethod;
};

#endif	/* MESH_H */

