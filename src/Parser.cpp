#include "Parser.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <string>

using namespace std;

Parser::Parser() {
}

Parser::~Parser() {
}

void Parser::Reading(const char * fileName) {
		ifstream cfg(fileName, ios::in);
        string scfg;
		int i;
        
        cfg >> scfg;
        cfg >> scfg;
		rheology = scfg.c_str();
		cfg >> scfg;
		cfg >> scfg;
		Left = scfg.c_str();
		cfg >> scfg;
		cfg >> scfg;
		Right = scfg.c_str();
		cfg >> scfg;
		cfg >> scfg;
        NumX = atoi(scfg.c_str());
        
        for(i = 0; i < 6; i++){
                cfg >> scfg;
                cfg >> scfg;
                body[i] = atof(scfg.c_str());
        }
        
        cfg >> scfg;
        for(i = 0; i < 6; i++){
            cfg >> scfg;
            cfg >> scfg;
            wave1[i] = atof(scfg.c_str());
        }
        
        cfg >> scfg;
        for(i = 0; i < 6; i++){
            cfg >> scfg;
            cfg >> scfg;
            wave2[i] = atof(scfg.c_str());
        }
        cfg.close();
}
    
Node *Parser::getInitValues() {
	float h = (body[1] - body[0])/(NumX - 1);
	InitValues = new Node [NumX];
    for (int i = 0; i < NumX; i++) {
		InitValues[i].num = i;
		InitValues[i].x = body[0] + i*h;
		InitValues[i].v = body[4];
		InitValues[i].eps = body[5];
		InitValues[i].E = body[3];
		InitValues[i].rho = body[2];
	}
	if (wave1[1] < wave1[0]) return InitValues;
	for (int i = 0; i < NumX; i++) {
		if ((InitValues[i].x >= wave1[0]) && (InitValues[i].x <= wave1[1])) {
			InitValues[i].v = wave1[4];
			InitValues[i].eps = wave1[5];
			InitValues[i].E = wave1[3];
			InitValues[i].rho = wave1[2];
		}
	}
	
	if (wave2[1] < wave2[0]) return InitValues;
	for (int i = 0; i < NumX; i++) {
		if ((InitValues[i].x >= wave2[0]) && (InitValues[i].x <= wave2[1])) {
			InitValues[i].v = wave2[4];
			InitValues[i].eps = wave2[5];
			InitValues[i].E = wave2[3];
			InitValues[i].rho = wave2[2];
		}
	}
	return InitValues;
}

int Parser::getNumX() {
	return NumX;
}
const char *Parser::getRheology() {
	return rheology;
}
const char *Parser::getLeft() {
	return Left;
}
const char *Parser::getRight() {
	return Right;
}
