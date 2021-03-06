#ifndef PARSER_H
#define	PARSER_H
#include "Node.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cfloat>

using namespace std;

// Parsing body1.txt and return initial values in the body
struct CnrCondition {
	double t1, t2;
	string type1, type2;
	bool isEps1, isEps2;
	double val1, val2;
};

class Parser {
        int NumX;
		string rheology;
        double body[6];
        double wave1[6];
        double wave2[6];
		struct CnrCondition Left;
		struct CnrCondition Right;
        Node *InitValues;
public:
        Parser();
        ~Parser();
        Node *getInitValues(); 
		int Reading(const char * fileName);
		int getNumX();
		string getRheology();
		void setGauss(double a, double sigma, int signOfInv, double *wave);
		struct CnrCondition getCnrCondition(bool isLeft);
};

#endif	/* PARSER_H */

