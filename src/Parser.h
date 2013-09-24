#ifndef PARSER_H
#define	PARSER_H
#include "Node.h"
#include <fstream>
#include <stdlib.h>
#include <sstream>

using namespace std;
// Parsing config.txt and return initial values in the body
class Parser {
        int NumX;
		string rheology, Left, Right;
        float body[6];
        float wave1[6];
        float wave2[6];
        Node *InitValues;
public:
        Parser();
        ~Parser();
        Node *getInitValues(); 
		void Reading(const char * fileName);
		int getNumX();
		string getRheology();
		string getLeft();
		string getRight();
};

#endif	/* PARSER_H */

