#include "Parser.h"

Parser::Parser() {
}

Parser::~Parser() {
}

int Parser::Reading(const char * fileName) {
		ifstream cfg(fileName, ios::in);
        string scfg;
		
        cfg >> scfg;
        cfg >> scfg; 
		rheology = scfg;
		cfg >> scfg;
		cfg >> scfg;
		cfg >> scfg;
		Left.t1 = atof(scfg.c_str());
		cfg >> Left.type1;
		cfg >> scfg;
		if (scfg.c_str()[0] == 'V') Left.isEps1 = false;
		if (scfg.c_str()[0] == 'E') Left.isEps1 = true;
		cfg >> scfg;
		Left.val1 = atof(scfg.c_str());
		cfg >> scfg;
		
		cfg >> scfg;
		cfg >> scfg;
		cfg >> scfg;
		Right.t1 = atof(scfg.c_str());
		cfg >> Right.type1;
		cfg >> scfg;
		if (scfg.c_str()[0] == 'V') Right.isEps1 = false;
		if (scfg.c_str()[0] == 'E') Right.isEps1 = true;
		cfg >> scfg;
		Right.val1 = atof(scfg.c_str());
		cfg >> scfg;

		cfg >> scfg;
		cfg >> scfg;
        NumX = atoi(scfg.c_str());
		InitValues = new Node [NumX];
        
        for(int i = 0; i < 6; i++){
                cfg >> scfg;
                cfg >> scfg;
                body[i] = atof(scfg.c_str());
        }
		double h = (body[1] - body[0])/(NumX - 1);
		for (int i = 0; i < NumX; i++) {
			InitValues[i].num = i;
			InitValues[i].x = body[0] + i*h;
			InitValues[i].v = body[4];
			InitValues[i].eps = body[5];
			InitValues[i].E = body[3];
			InitValues[i].rho = body[2];
			InitValues[i].rheology = rheology;
		}
	
        cfg >> scfg;
        cfg >> scfg;
		if ((scfg.c_str())[0] == 'g') {
			cfg >> scfg;
			int signOfInv = atoi(scfg.c_str());
			if (signOfInv == 2) signOfInv = -1;
			cfg >> scfg;
			double a = atof(scfg.c_str());
			cfg >> scfg;
			double sigma = atof(scfg.c_str());
			for(int i = 0; i < 6; i++){
				cfg >> scfg;
				cfg >> scfg;
				wave1[i] = atof(scfg.c_str());
			}
			setGauss(a, sigma, signOfInv, wave1);
		}
		else {
				cfg >> scfg;
				cfg >> scfg;
				cfg >> scfg;
			for(int i = 0; i < 6; i++){
				cfg >> scfg;
				cfg >> scfg;
			    wave1[i] = atof(scfg.c_str());
			}
			if (wave1[1] >= wave1[0]) {
			for (int i = 0; i < NumX; i++) {
			if ((InitValues[i].x >= wave1[0]) && (InitValues[i].x <= wave1[1])) {
				InitValues[i].v = wave1[4];
				InitValues[i].eps = wave1[5];
				InitValues[i].E = wave1[3];
				InitValues[i].rho = wave1[2];
			}}}
		}
        
        cfg >> scfg;
        cfg >> scfg;
		if ((scfg.c_str())[0] == 'g') {
			cfg >> scfg;
			int signOfInv = atoi(scfg.c_str());
			if (signOfInv == 2) signOfInv = -1;
			cfg >> scfg;
			double a = atof(scfg.c_str());
			cfg >> scfg;
			double sigma = atof(scfg.c_str());
			for(int i = 0; i < 6; i++){
				cfg >> scfg;
				cfg >> scfg;
				wave2[i] = atof(scfg.c_str());
			}
			setGauss(a, sigma, signOfInv, wave2);
		}
		else {
				cfg >> scfg;
				cfg >> scfg;
				cfg >> scfg;
			for(int i = 0; i < 6; i++){
				cfg >> scfg;
				cfg >> scfg;
			    wave2[i] = atof(scfg.c_str());
			}
			if (wave2[1] >= wave2[0]) {
			for (int i = 0; i < NumX; i++) {
			if ((InitValues[i].x >= wave2[0]) && (InitValues[i].x <= wave2[1])) {
				InitValues[i].v = wave2[4];
				InitValues[i].eps = wave2[5];
				InitValues[i].E = wave2[3];
				InitValues[i].rho = wave2[2];
			}}}
		}
		
        cfg.close();
		
		// Layer structure
//		
//		double rho0 = body[2];
//		int NumOfLayers = 5;
//		int NumOfPeriods = 1;
//		int a = NumX/NumOfPeriods;
//		int b = a/NumOfLayers;
//		int m,i;
//		for (int n = 0; n < NumOfPeriods; n++) {
//			for (m = 0; m < NumOfLayers; m++) {
//				for (i = a*n + b*m; i < a*n + b*(m+1); i++) {
//					InitValues[i].rho =	rho0*(m+1);
//				} 
//			}
//			while(i < NumX) {
//				InitValues[i].rho =	rho0*(m);
//				i++;
//			}
//		}

		// Layer structure

}
    
Node *Parser::getInitValues() {
	return InitValues;
}

void Parser::setGauss(double a, double sigma, int signOfInv, double *wave) {
	double A = sqrt(body[3]/body[2]);
	for (int i = 0; i < NumX; i++) {
		if ((InitValues[i].x >= wave[0]) && (InitValues[i].x <= wave[1])) {
			InitValues[i].v = wave[4] * \
				exp(-(InitValues[i].x - a)*(InitValues[i].x - a)/2/(sigma*sigma));
			InitValues[i].eps = - InitValues[i].v / A * signOfInv;
		}}
}

int Parser::getNumX() {
	return NumX;
}
string Parser::getRheology() {
	return rheology;
}
struct CnrCondition Parser::getCnrCondition(bool isLeft) {
	if (isLeft) return Left;
	return Right;
}
