#ifndef PARSER_H
#define	PARSER_H
#include "Node.h"

// Parsing config.txt and return initial values in the body
class Parser {
        int NumX;
		const char *rheology, *Left, *Right;
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
		const char *getRheology();
		const char *getLeft();
		const char *getRight();
};

#endif	/* PARSER_H */

